#include "mmconvert.h"
#include "ui_mmconvert.h"

#include "mmconvert.h"
#include <QMessageBox>
#include <QFileDialog>

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QStandardItemModel>

#include "dialogfinddupes.h"
#include "dialogimportmovies.h"
#include "dialogmoviecompare.h"
#include "cboxsimpledelegate.h"
#include "filefindsimpledelegate.h"
#include "spinsimpledelegate.h"

#include "mmsettings.h"
#include "dialogmmsettings.h"
#include "dialogdeepprobe.h"
#include "dialogplexdbscanner.h"



MMConvert::MMConvert(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MMConvert)
{
    _convertRunning = false;

    ui->setupUi(this);
    setAcceptDrops(true);
    ui->lblStatus->setStyleSheet("QLabel { background-color : yellow; color : black ; }");

    if(MMSettings::getInstance()->loadSettings() == false)
        on_actionMM_Settings_triggered();

    _ffMpegInvoker = new ffMpegInvoker();
    connect(_ffMpegInvoker, SIGNAL(processUpdate(QString, MMConversionInfo*)), this, SLOT(threadProcessUpdate(QString, MMConversionInfo*)),Qt::QueuedConnection);
    connect(_ffMpegInvoker, SIGNAL(processComplete(MMConversionInfo*)), this, SLOT(processComplete(MMConversionInfo*)),Qt::QueuedConnection);

    _dirWatcher = new MMDirWatcher(MMSettings::getInstance()->getDirToMonitor(), this);
    connect(_dirWatcher, SIGNAL(fileChangeNotice(QString, MMDirWatcher::FileStatus, quint64)), this, SLOT(changeNotice(QString, MMDirWatcher::FileStatus, quint64)),Qt::QueuedConnection);
    //connect(_dirWatcher, SIGNAL(fileChangeNotice(QString,FileStatus,quint64)), this, SLOT(changeNotice(QString,MMDirWatcher::FileStatus,quint64)), Qt::QueuedConnection);
    _dirWatcher->start();

    QSqlDatabase  database = QSqlDatabase().addDatabase("QMYSQL","mmqueue");
    database.setUserName(MMSettings::getInstance()->getUser());
    database.setPassword(MMSettings::getInstance()->getPwd());
    database.setDatabaseName(MMSettings::getInstance()->getDataBase());
    database.setHostName(MMSettings::getInstance()->getServer());
    database.open();

    _player = new QMediaPlayer;

    _playlist = new QMediaPlaylist(_player);

    _videoWidget = new QVideoWidget;
    _player->setVideoOutput(_videoWidget);

    _mmConvertModel = new  TableModel(this, database);
    _mmConvertModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    _mmConvertModel->setTable("mmqueue");
    _mmConvertModel->setFilter("wasProcessed = 0 and wasRemoved = 0");
    _mmConvertModel->select();


    ui->tvDB->setModel(_mmConvertModel);
    ui->tvDB->viewport()->installEventFilter(this);
    ui->tvDB->setMouseTracking(true);

    ui->tvDB->horizontalHeader()->setVisible(true);
    ui->tvDB->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    _popup = new QDialog(this, Qt::Popup | Qt::ToolTip);

   QVBoxLayout *layout = new QVBoxLayout;
   _popupLabel = new QLabel(_popup);
   _popupLabel->setWordWrap(true);
   layout->addWidget(_popupLabel);
   _popupLabel->setTextFormat(Qt::RichText);
   _popupLabel->setOpenExternalLinks(true);
   _popup->setLayout(layout);
   _popup->installEventFilter(this);


    //set visibal rows / set column Styles
    for (int counter = 0; counter < _mmConvertModel->columnCount(); counter++)
    {
        ui->tvDB->setColumnHidden(counter, TableModel::getHideColumn(counter));

        switch(counter)
        {
            case preset:
                ui->tvDB->setItemDelegateForColumn(preset, new CBoxSimpleDelegate(MMConversionInfo::getPresets(),this));
            break;

            case outputDir:
                ui->tvDB->setItemDelegateForColumn(outputDir, new FileFindSimpleDelegate(true,this));
            break;

            case crf:
                ui->tvDB->setItemDelegateForColumn(crf, new SpinSimpleDelegate(0, 51,this));
            break;

        }

    }


    ui->tvDB->show();

    setControls();

}


void MMConvert::changeNotice(QString fileName, MMDirWatcher::FileStatus stat, quint64 lastSize)
{
    bool add = true;

    if(lastSize <= 0 || stat == MMDirWatcher::Deleted) return;

    for(int counter = 0; counter < _mmConvertModel->rowCount(); counter++)
    {
        QString name;

        name.append(_mmConvertModel->index(counter,  ColumInfo::filePath).data().toString());
        name.append("/");
        name.append(_mmConvertModel->index(counter,  ColumInfo::fileName).data().toString());

        if(name == fileName)
        {
            add = false;
            break;
        }
    }

    if(add)
    {
        _mmInfo.clear();
        _mmInfo.setFullFile(fileName);
        ProbeMovie();
        _mmInfo.setProcess(true);
        addMM();
        processSelected();
    }
}
//10.2.0.25

MMConvert::~MMConvert()
{
    _dirWatcher->stop();
    _dirWatcher->wait();
    delete _dirWatcher;
    delete _mmConvertModel;
    delete _ffMpegInvoker;
    delete ui;
}


void MMConvert::setControls()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->gbQueue->setTitle("Queue: " + QString::number(_mmConvertModel->rowCount()));

}


void MMConvert::processComplete(MMConversionInfo* mmInfo)
{
    std::stringstream info;

    info << "Finished Converting File: " << mmInfo->fileName().toStdString().c_str() <<
            " in " << (mmInfo->secProcessing() / 60 ) << " Minutes.";
    ui->lblStatus->setText(info.str().c_str());

    ui->progressBar->setValue(100);
    _mmConvertModel->updateData(mmInfo);
    _mmConvertModel->select();

    if(mmInfo->deleteSource())
    {
        QFile file(mmInfo->inFullFileName());
        if(file.exists())
            file.remove();
    }

    ui->gbQueue->setTitle("Queue: " + QString::number(_mmConvertModel->rowCount()));
    delete mmInfo;

    _convertRunning = false;
    if(ui->gbQueue->isChecked() == true)
        processSelected();
    else
        ui->gbQueue->setChecked(true);


}

void MMConvert::threadProcessUpdate(QString qMessage, MMConversionInfo *mmInfo)
{

    double progress = mmInfo->getCurrentProgressValue();
    double finishedProgress = mmInfo->getFormatDuration();

    double value = 0;
    value = ((progress / finishedProgress) * 100.00);

    value = (value < 1) ? 0 : value;
    ui->progressBar->setValue(value);
    ui->lblStatus->setText(qMessage);

}



void MMConvert::setMMInfo(int index, MMConversionInfo *mmInfo)
{
    mmInfo->clear();
    mmInfo->setDbRecId(_mmConvertModel->record(index).value("idmmqueue").toLongLong());
    mmInfo->setFileName(_mmConvertModel->record(index).value("fileName").toString());
    mmInfo->setfilePath(_mmConvertModel->record(index).value("filePath").toString());
    mmInfo->setOutDir(_mmConvertModel->record(index).value("outputDir").toString());
    mmInfo->setContainer(_mmConvertModel->record(index).value("container").toString());
    mmInfo->setIsProcessed(_mmConvertModel->record(index).value("wasProcessed").toBool());
    mmInfo->setIsRemoved(_mmConvertModel->record(index).value("wasRemoved").toBool());
    mmInfo->setSecProcessing(_mmConvertModel->record(index).value("secProcessing").toInt());
    mmInfo->setDeleteSource(_mmConvertModel->record(index).value("wasDeleted").toBool());
    mmInfo->setUseCustomSettings(_mmConvertModel->record(index).value("useCustomSettings").toBool());
    mmInfo->setPreset(_mmConvertModel->record(index).value("preset").toString());
    mmInfo->setCrf(_mmConvertModel->record(index).value("crf").toInt());
    mmInfo->setStripSubs(_mmConvertModel->record(index).value("stripSubs").toBool());
    mmInfo->setStreamsMetaInfo(_mmConvertModel->record(index).value("streamsInfo").toString());
    mmInfo->setConversionError(_mmConvertModel->record(index).value("conversionErrorCode").toInt());
    mmInfo->setProcess(_mmConvertModel->record(index).value("process").toBool());
    mmInfo->setStartFileSize(_mmConvertModel->record(index).value("startFileSize").toLongLong());
    mmInfo->setEndFileSize(_mmConvertModel->record(index).value("endFileSize").toLongLong());
    mmInfo->setFormatDuration(_mmConvertModel->record(index).value("formatDuration").toLongLong());
}



void MMConvert::addMM()
{
    _mmConvertModel->updateData(&_mmInfo);
    _mmConvertModel->select();
    _mmInfo.clear();
    setControls();
}


void MMConvert::ProbeMovie()
{
    ffMpegInvoker::probeMM(&_mmInfo);
    long check = MMSettings::getInstance()->getAutocustomTrigger();

    check *= 1024;
    check *= 1024;
    check *= 1024;
    if(_mmInfo.getStartFileSize() > check)
        _mmInfo.setUseCustomSettings(true);

    setControls();

}

void MMConvert::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}


//TODO: Limit file drops to just what we allow.
//"*.mkv *.avi *.mov *.mp4"
void MMConvert::dropEvent(QDropEvent *event)
{
    foreach (const QUrl &url, event->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        _mmInfo.clear();
        _mmInfo.setFullFile(fileName);
        ProbeMovie();
        addMM();
    }
}

void MMConvert::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

bool MMConvert::eventFilter(QObject *watched, QEvent *event)
{
    if(ui->tvDB->viewport() == watched)
    {

        if(event->type() == QEvent::Leave) _popup->hide();

        if(event->type() == QEvent::MouseMove)
        {

            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex index = ui->tvDB->indexAt(mouseEvent->pos());

            if(index.row() == -1 )
            {
                _popup->hide();
                return false;
            }

            if(TableModel::getColumnPopUp(index.column()) == true)
            {
                QString value;
                if(index.column() == fileName)
                {
                    QString fileName = index.data(Qt::DisplayRole).toString();
                    QString path = index.sibling(index.row(), filePath).data().toString();
                    value = path + "/" + fileName;

                }
                else
                {
                     value = index.data(Qt::DisplayRole).toString();
                }
                //_popupLabel->setOpenExternalLinks(true);

                //_popup->setFixedSize(fileName.length(), _popup->heightForWidth(fileName.length()));
                //_popupLabel->setText(index.data(Qt::DisplayRole).toString());
                //  _popup->adjustSize();
                QRect r = ui->tvDB->visualRect(index);
                _popup->move(ui->tvDB->viewport()->mapToGlobal(r.bottomLeft()));
                _popupLabel->setText(value);
                _popup->show();
            }
            else
            {
                _popup->hide();
            }
        }

    }

    return false;
}


void MMConvert::processSelected()
{
    if(true == _convertRunning)
        return;

    for(int counter = 0; counter < _mmConvertModel->rowCount(); counter++)
    {
        if(_mmConvertModel->record(counter).value(process).toBool() == true &&
                _mmConvertModel->record(counter).value(wasProcessed).toBool() == false &&
                _mmConvertModel->record(counter).value(wasRemoved).toBool() == false)
        {
            MMConversionInfo* mmInfo = new MMConversionInfo();
            setMMInfo(counter, mmInfo );
            if(mmInfo ->OutDir() != mmInfo->getFilePath())
            {
                _convertRunning = true;
                _ffMpegInvoker->setMmInfo(mmInfo);
                _ffMpegInvoker->start();
                break;
            }
            else
            {
                mmInfo->setProcess(false);
                _mmConvertModel->updateData(mmInfo);
                _mmConvertModel->select();

                delete mmInfo;
                //TODO: We need an Error Screen.  QListQidget comes to mind.

            }
        }
    }
}



void MMConvert::on_btnProcess_clicked()
{
    processSelected();
}

void MMConvert::on_actionFind_Dupes_triggered()
{
    DialogFindDupes dupes;
    dupes.exec();

}

void MMConvert::on_actionLoad_MegaBoom_triggered()
{
    DialogImportMovies import;
    import.exec();

}

void MMConvert::on_actionCompare_triggered()
{
    DialogMovieCompare compare;
    compare.exec();

}

void MMConvert::on_actionMM_Settings_triggered()
{
    DialogMMSettings settings;
    settings.exec();
}

void MMConvert::on_actionPlex_DB_Scan_triggered()
{
    DialogPlexDBScanner plexScanner;
    plexScanner.exec();
}



void MMConvert::on_tvDB_customContextMenuRequested(const QPoint &pos)
{

    //    QModelIndex index = ui->tvDB->indexAt(pos);

    QMenu *menu = new QMenu(this);
    QAction* probe = new QAction("Probe", this);
    connect(probe, SIGNAL(triggered(bool)), SLOT(probeFile()));

    QAction* play = new QAction("Play", this);
    connect(play, SIGNAL(triggered(bool)), SLOT(playFile()));


    QAction* process = new QAction("Mark For Process", this);
    connect(process, SIGNAL(triggered(bool)), SLOT(markProcess()));

    QAction* del = new QAction("Mark For Delete", this);
    connect(del, SIGNAL(triggered(bool)), SLOT(markDel()));

    QAction* custom = new QAction("Use Custom Settings", this);
    connect(custom, SIGNAL(triggered(bool)), SLOT(markUseCustom()));

    //menu->addAction(probe);
    menu->addAction(play);
    menu->addSeparator();
    menu->addAction(process);
    menu->addAction(custom);
    menu->addSeparator();
    menu->addAction(del);

    menu->popup(ui->tvDB->viewport()->mapToGlobal(pos));
}

void MMConvert::playFile()
{
    int row = ui->tvDB->selectionModel()->currentIndex().row();
    if(row >= 0)
    {
        QString fName =  _mmConvertModel->record(row).value(fileName).toString();
        QString fPath =  _mmConvertModel->record(row).value(filePath).toString();
        MMConversionInfo mmInfo;
        mmInfo.setFileName(fName);
        mmInfo.setfilePath(fPath);

        _playlist->clear();
        _playlist->addMedia(QUrl::fromLocalFile(mmInfo.inFullFileName()));

        _videoWidget->show();
        _player->play();

    }
}

void MMConvert::probeFile()
{
    int row = ui->tvDB->selectionModel()->currentIndex().row();
    if(row >= 0)
    {

        QString fName =  _mmConvertModel->record(row).value(fileName).toString();
        QString fPath =  _mmConvertModel->record(row).value(filePath).toString();

        MMConversionInfo mmInfo;
        mmInfo.setFileName(fName);
        mmInfo.setfilePath(fPath);

        DialogDeepProbe deepProbe(&mmInfo);
        deepProbe.exec();

    }
}


void MMConvert::markProcess()
{
    foreach(auto index, ui->tvDB->selectionModel()->selectedIndexes())
        if(index.row() >= 0)
            _mmConvertModel->updateColumn(index.row(), process, !_mmConvertModel->record(index.row()).value(process).toBool());

}


void MMConvert::markDel()
{
    foreach(auto index, ui->tvDB->selectionModel()->selectedIndexes())
        if(index.row() >= 0)
           _mmConvertModel->updateColumn(index.row(), wasDeleted, !_mmConvertModel->record(index.row()).value(wasDeleted).toBool());

}


void MMConvert::markUseCustom()
{
    foreach(auto index, ui->tvDB->selectionModel()->selectedIndexes())
        if(index.row() >= 0)
            _mmConvertModel->updateColumn(index.row(), useCustomSettings, !_mmConvertModel->record(index.row()).value(useCustomSettings).toBool());

}

