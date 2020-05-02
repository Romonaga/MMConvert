#include "dialogplexdbscanner.h"
#include "ui_dialogplexdbscanner.h"
#include <QString>
#include <QMouseEvent>
#include <QSqlRecord>
#include <QFileDialog>
#include <QModelIndex>
#include <QMap>
#include <QVariant>
#include <QFile>

DialogPlexDBScanner::DialogPlexDBScanner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPlexDBScanner)
{
    ui->setupUi(this);

    ui->tvPlex->viewport()->installEventFilter(this);
    ui->tvPlex->setMouseTracking(true);
    _popup = new QDialog(this, Qt::Popup | Qt::ToolTip);

   QVBoxLayout *layout = new QVBoxLayout;
   _popupLabel = new QLabel(_popup);
   _popupLabel->setWordWrap(true);
   layout->addWidget(_popupLabel);
   _popupLabel->setTextFormat(Qt::RichText);
   _popupLabel->setOpenExternalLinks(true);
   _popup->setLayout(layout);
   _popup->installEventFilter(this);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "scanner");
    db.setDatabaseName("/var/lib/plexmediaserver/Library/Application Support/Plex Media Server/Plug-in Support/Databases/com.plexapp.plugins.library.db");
    if(db.open())
    {
        _plexModel = new PlexMediaTableModel(this, db);
        ui->tvPlex->setModel(_plexModel);
        _plexModel->select();

        //set visibal rows / set column Styles
        for (int counter = 0; counter < _plexModel->columnCount(); counter++)
        {
            ui->tvPlex->setColumnHidden(counter, PlexMediaTableModel::getHideColumn(counter));


        }



    }
}




DialogPlexDBScanner::~DialogPlexDBScanner()
{
    delete ui;
}



bool DialogPlexDBScanner::eventFilter(QObject *watched, QEvent *event)
{
    if(ui->tvPlex->viewport() == watched)
    {

        if(event->type() == QEvent::Leave) _popup->hide();

        if(event->type() == QEvent::MouseMove)
        {

            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex index = ui->tvPlex->indexAt(mouseEvent->pos());

            if(index.row() == -1 )
            {
                _popup->hide();
                return false;
            }

            if(PlexMediaTableModel::getColumnPopup(index.column()) == true)
            {
                QString summery = index.data(Qt::DisplayRole).toString();
                QRect r = ui->tvPlex->visualRect(index);
                _popup->move(ui->tvPlex->viewport()->mapToGlobal(r.bottomLeft()));
                _popupLabel->setText(summery);
                _popup->show();
            }
            else
                _popup->hide();
        }

    }
    return false;
}

void DialogPlexDBScanner::on_btnCreateNewDirStructure_clicked()
{
    QString dir;
    QDir qdir;
    bool dryRun = false;


    if(ui->tvPlex->selectionModel()->selectedRows().count() == 0) return;

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, true);
    dryRun = ui->cboxDryRun->isChecked();

    fileDialog.exec();
    if(fileDialog.result() == QDialog::Accepted)
    {
        dir = fileDialog.selectedFiles().at(0);
        ui->listStatus->addItem("Directory Selected: " + dir);
        foreach(QString genre,  _plexModel->getParentGenre())
        {
            QString cDir;
            cDir.append(dir);
            cDir.append("/");
            cDir.append(genre);
            if(qdir.mkpath(cDir) == false)
            {
                ui->listStatus->addItem("Create Failed: " + cDir + " Halting.");
                return;
            }
            else
            {
                ui->listStatus->addItem("Created Dir: " + cDir + ".");
            }
        }

        QMap<QString, QString> plex = _plexModel->getFileGenre();

        foreach(QModelIndex index, ui->tvPlex->selectionModel()->selectedRows())
        {
            QString file = _plexModel->index(index.row(), PlexInfo::file).data().toString();

            if(plex.contains(file))
            {

                QString genre = plex[file];
                QString newPath = dir + "/" + genre + "/" + QFileInfo(file).fileName();
                if(false == dryRun)
                {
                    QFile(file).rename(newPath);
                    ui->listStatus->addItem("Went From: " + file + " To: " + newPath + ".");
                }
                else
                {
                    ui->listStatus->addItem("Would Have! Went From: " + file + " To: " + newPath + ".");
                }

            }

        }

    }


}
