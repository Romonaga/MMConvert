#include "dialogfinddupes.h"
#include "ui_dialogfinddupes.h"
#include <QDirIterator>
#include <QMap>
#include <QTreeWidget>
#include <QDateTime>
#include <QFileDialog>



DialogFindDupes::DialogFindDupes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFindDupes)
{
    ui->setupUi(this);
}

DialogFindDupes::~DialogFindDupes()
{
    delete ui;
}

void DialogFindDupes::runDupeCheck()
{
    QMap<QString, QStringList> dupChecks;

    QDirIterator dirWalk(_dirToScan, QDir::NoFilter, QDirIterator::Subdirectories);

    ui->treeDupes->clear();
    while(dirWalk.hasNext())
    {
        QString nextItr = dirWalk.next();
        QFileInfo file(nextItr);
        if(file.exists() && file.isFile())
        {
            if(dupChecks.contains(file.completeBaseName()))
            {
                dupChecks[file.completeBaseName()].append(file.absoluteFilePath());
            }
            else
            {
                QList<QString> list;
                list.append(file.absoluteFilePath());
                dupChecks.insert(file.completeBaseName(), list);
            }
        }
    }

    foreach(QString fileMap, dupChecks.keys())
    {
        if(dupChecks[fileMap].count() > 1)
        {
            QTreeWidgetItem* dupFileName = new QTreeWidgetItem();
            dupFileName->setText(0,fileMap);
            foreach(QString file, dupChecks[fileMap])
            {
                QTreeWidgetItem* tfile = new QTreeWidgetItem();
                tfile->setFlags(tfile->flags() | Qt::ItemIsUserCheckable);
                tfile->setCheckState(0, Qt::Unchecked);
                tfile->setText(0, file);

                QTreeWidgetItem* fileitem = new QTreeWidgetItem();
                QFileInfo fileInfo(file);
                QString sfileInfo;
                sfileInfo.append("Size: ");
                sfileInfo.append(QLocale(QLocale::English).toString(fileInfo.size()));
                sfileInfo.append(" Date: ");
                sfileInfo.append(fileInfo.created().toString());
                fileitem->setText(0,sfileInfo);
                tfile->addChild(fileitem);

                dupFileName->addChild(tfile);

            }

            ui->treeDupes->insertTopLevelItem(ui->treeDupes->topLevelItemCount(), dupFileName);
        }
    }
}

void DialogFindDupes::on_btnRun_clicked()
{

    QFileDialog dirDialog;
    dirDialog.setFileMode(QFileDialog::Directory);
    dirDialog.setOption(QFileDialog::ShowDirsOnly, true);

    _dirToScan = dirDialog.getExistingDirectory(this, "Select Directory");

    runDupeCheck();

}

void DialogFindDupes::on_btnDelete_clicked()
{
    for( int counter = 0; counter < ui->treeDupes->topLevelItemCount(); counter++ )
    {
       QTreeWidgetItem *item = ui->treeDupes->topLevelItem(counter);
       for(int ccounter = 0; ccounter < item->childCount(); ccounter++)
       {
           if(item->child(ccounter)->checkState(0))
           {
                QFile file(item->child(ccounter)->data(0, 0).toString());
                if(file.exists()) file.remove();
           }
       }

    }

    runDupeCheck();
}


void DialogFindDupes::on_treeDupes_itemExpanded(QTreeWidgetItem *item)
{
    for(int counter =0; counter < item->childCount(); counter++)
        if(item->child(counter)->isExpanded() == false)
            item->child(counter)->setExpanded(true);

}
