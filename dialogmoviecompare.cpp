#include "dialogmoviecompare.h"
#include "ui_dialogmoviecompare.h"
#include <sstream>
#include <QDirIterator>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlField>
#include <QFileDialog>
#include <QVariant>

#include <QDebug>
#include <QListWidgetItem>
#include "mmsettings.h"
#include "mmconvertcommon.h"
#include "mmconversioninfo.h"
#include "ffmpeginvoker.h"

DialogMovieCompare::DialogMovieCompare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMovieCompare)
{
    ui->setupUi(this);
}

DialogMovieCompare::~DialogMovieCompare()
{
    delete ui;
}

void DialogMovieCompare::on_btnRun_clicked()
{
    QMap<QString, QString> dbList;
    QStringList fsList;
    QString movieName;
    QString movieType;
    QStringList movieNameParts;
    bool isRip = false;


    this->setWindowTitle("MegaBoon History - Files");
    QFileDialog dirDialog;

    dirDialog.setFileMode(QFileDialog::Directory);
    dirDialog.setOption(QFileDialog::ShowDirsOnly, true);


    QString dirName = dirDialog.getExistingDirectory(this, "Select Directory");
    QSqlDatabase  database = QSqlDatabase().addDatabase("QMYSQL", "mmMegaBoom");
    database.setUserName(MMSettings::getInstance()->getUser());
    database.setPassword(MMSettings::getInstance()->getPwd());
    database.setDatabaseName(MMSettings::getInstance()->getDataBase());
    database.setHostName(MMSettings::getInstance()->getServer());

    if(database.open())
    {
        ui->listResults->clear();
        QSqlQuery qry = database.exec("select * from mmMegaBoom where dupe = 0 order by movieName asc");
        while(qry.next())
        {
            movieName =  qry.value(1).toString();
            movieName.append("-");
            movieName.append(qry.value(2).toString());
            movieName.replace("\"","");
            movieName.replace(": ","_");
            movieName.replace("&","and");

            dbList.insert(movieName.replace(" " ,""), qry.value(1).toString());


        }

        database.close();
        QDirIterator dirWalk(dirName, QDir::NoFilter, QDirIterator::Subdirectories);
        while(dirWalk.hasNext())
        {
            QString nextItr = dirWalk.next();
            QFileInfo file(nextItr);
            if(file.exists() && file.isFile())
            {
                movieName.clear();
                movieType.clear();

                movieNameParts = file.completeBaseName().split('-', QString::SplitBehavior::SkipEmptyParts);
                if(movieNameParts.length() >= 3 )
                {

                    isRip = (movieNameParts[movieNameParts.length() - 1] == "rip");
                    if(isRip)
                    {
                        movieType.append(movieNameParts[movieNameParts.length() - 2]);
                        movieType.append("-");
                        movieType.append(movieNameParts[movieNameParts.length() - 1]);
                    }
                    else
                    {
                        movieType.append(movieNameParts[movieNameParts.length() - 1]);

                    }

                    if(movieNameParts.length() == 3 )
                    {
                        movieName.append(movieNameParts[0]);
                        movieName.append("-");
                    }
                    else
                    {
                        int offset = ((isRip) ? movieNameParts.length() - 1 : 2) ;
                        for(int counter =  0; counter < (movieNameParts.length() - offset); counter++)
                        {
                            movieName.append(movieNameParts[counter]);
                            movieName.append("-");
                        }
                    }

                    movieName.append(movieType);
                    movieName.replace("&","and");
                    movieName.replace(',',"");
                    fsList.append(movieName.replace(" " ,""));
                }

            }

        }

        fsList.sort();
        int hitCount = 0;
        foreach(QString fsFile, fsList)
        {
            if(dbList.contains(fsFile) == true)
            {
                hitCount++;
                dbList.remove(fsFile);
            }

        }

        QIcon icon(":/art/icons8-film-reel-48.png");
        QListWidgetItem* tot = new QListWidgetItem(icon,"Number Missing: " + QString::number(dbList.size()));
        ui->listResults->addItem(tot);

        foreach(QString fName, dbList.values())
            ui->listResults->addItem(fName);

    }


}

void DialogMovieCompare::on_btnMega_clicked()
{
    QStringList plexFiles;
    QString dir;
    QFileDialog fileDialog;

    this->setWindowTitle("Plex Media - Files");
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly, true);

    fileDialog.exec();
    if(fileDialog.result() == QDialog::Accepted)
    {
        ui->listResults->clear();
        dir = fileDialog.selectedFiles().at(0);

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "compare");
        db.setDatabaseName("/var/lib/plexmediaserver/Library/Application Support/Plex Media Server/Plug-in Support/Databases/com.plexapp.plugins.library.db");
        if(db.open())
        {
               QSqlQuery qry = db.exec("select file from media_parts where media_parts.directory_id > 0");

               while(qry.next())
               {
                   QString fileName = qry.value(0).toString();

                   plexFiles.append(fileName);
               }

                QDirIterator dirWalk(dir, QDir::NoFilter, QDirIterator::Subdirectories);
                while(dirWalk.hasNext())
                {
                    QString dirFile = dirWalk.next();
                    if( QFileInfo(dirFile).isFile() == false) continue;

                    if(plexFiles.contains(dirFile) == false)
                         ui->listResults->addItem("Plex Missing: " + dirFile);
                }

            db.close();
        }
    }
}


void DialogMovieCompare::on_btnMegaDL_clicked()
{
    QMap<QString, QString> dbList;
    QStringList fsList;
    QString movieName;
    QString movieType;
    QStringList movieNameParts;
    bool isRip = false;

    this->setWindowTitle("MegaBoon DL List - Files");

    QFileDialog dirDialog;

    dirDialog.setFileMode(QFileDialog::Directory);
    dirDialog.setOption(QFileDialog::ShowDirsOnly, true);


    QString dirName = dirDialog.getExistingDirectory(this, "Select Directory");
    QSqlDatabase  database = QSqlDatabase().addDatabase("QMYSQL", "mmMegaDLList");
    database.setUserName(MMSettings::getInstance()->getUser());
    database.setPassword(MMSettings::getInstance()->getPwd());
    database.setDatabaseName(MMSettings::getInstance()->getDataBase());
    database.setHostName(MMSettings::getInstance()->getServer());

    if(database.open())
    {
        ui->listResults->clear();
        QSqlQuery qry = database.exec("select * from mmMegaDLList order by movieName asc");
        while(qry.next())
        {
            movieName =  qry.value(1).toString();
            movieName = movieName.left(movieName.indexOf('('));
            movieName.replace("\"","");
            movieName.replace("-","");
            movieName.replace(",","");
            movieName.replace(": ","_");
            movieName.replace("&","and");

            dbList.insert(movieName.replace(" " ,""), qry.value(1).toString());


        }

        database.close();
        QDirIterator dirWalk(dirName, QDir::NoFilter, QDirIterator::Subdirectories);
        while(dirWalk.hasNext())
        {
            QString nextItr = dirWalk.next();
            QFileInfo file(nextItr);
            if(file.exists() && file.isFile())
            {
                movieName.clear();

                movieNameParts = file.completeBaseName().split('-', QString::SplitBehavior::SkipEmptyParts);
                if(movieNameParts.length() >= 3 )
                {
                    isRip = (movieNameParts[movieNameParts.length() - 1] == "rip");
                    if(movieNameParts.length() == 3 )
                        movieName.append(movieNameParts[0]);
                    else
                    {
                        int offset = ((isRip) ? movieNameParts.length() - 1 : 2) ;
                        for(int counter =  0; counter < (movieNameParts.length() - offset); counter++)
                            movieName.append(movieNameParts[counter]);
                    }

                    movieName.replace("&","and");
                    movieName.replace(',',"");
                    fsList.append(movieName.replace(" " ,""));
                }

            }

        }

        fsList.sort();
        int hitCount = 0;
        foreach(QString fsFile, fsList)
        {
            if(dbList.contains(fsFile) == true)
            {
                hitCount++;
                dbList.remove(fsFile);
            }

        }

        QIcon icon(":/art/icons8-film-reel-48.png");
        QListWidgetItem* tot = new QListWidgetItem(icon,"Number Missing: " + QString::number(dbList.size()));
        ui->listResults->addItem(tot);

        foreach(QString fName, dbList.values())
            ui->listResults->addItem(fName);

    }


}
