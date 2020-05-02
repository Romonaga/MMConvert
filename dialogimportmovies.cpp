#include "dialogimportmovies.h"
#include "ui_dialogimportmovies.h"
#include <sstream>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "mmsettings.h"


DialogImportMovies::DialogImportMovies(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImportMovies)
{
    ui->setupUi(this);
}

DialogImportMovies::~DialogImportMovies()
{
    delete ui;
}

void DialogImportMovies::on_btnSelectFile_clicked()
{
    QStringList fileLines;
    QStringList movieInfo;
    std::stringstream sql;
    QSqlQuery result;

    //todo conf file for this as well as settings screen.
    QSqlDatabase  database = QSqlDatabase().addDatabase("QMYSQL","mmMegaBoom");
    database.setHostName(MMSettings::getInstance()->getServer());
    database.setUserName(MMSettings::getInstance()->getUser());
    database.setPassword(MMSettings::getInstance()->getPwd());
    database.setDatabaseName(MMSettings::getInstance()->getDataBase());
    if(database.open())
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Import File");
        QFile inputFile(fileName);

        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
              fileLines.append(in.readLine());

            inputFile.close();

            if(fileLines.length() > 0)
            {
                sql.str("");
                sql << "truncate table mmMegaBoom";
                database.exec(sql.str().c_str());

                foreach(QString movie, fileLines)
                {

                    sql.str("");
                    movieInfo = movie.split(',', QString::SplitBehavior::SkipEmptyParts);
                    sql << "insert into mmMegaBoom(ID, movieName, movieType, purchaseAmount, purchaseType) values(" <<
                           atoi(movieInfo[0].toStdString().c_str()) << ",'" <<
                           movieInfo[1].toStdString().c_str() << "','" <<
                           movieInfo[2].toStdString().c_str() << "','" <<
                           movieInfo[3].toStdString().c_str() << "','" <<
                           movieInfo[4].toStdString().c_str() << "')";

                    result = database.exec(sql.str().c_str());


                }


            }
        }

       database.close();
    }

}