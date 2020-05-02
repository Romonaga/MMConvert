#include "dialogdeepprobe.h"
#include "ui_dialogdeepprobe.h"
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QDebug>

DialogDeepProbe::DialogDeepProbe(MMConversionInfo* mmInfo, QWidget *parent) : _mmInfo(mmInfo),
    QDialog(parent),
    ui(new Ui::DialogDeepProbe)
{
    ui->setupUi(this);


   // "media_item_count, title, studio, rating, rating_count, tagline, summary, quotes, content_rating, content_rating_age,tags_genre,tags_director,tags_writer,tags_star,originally_available_at, year,tags_country,audience_rating"

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/var/lib/plexmediaserver/Library/Application Support/Plex Media Server/Plug-in Support/Databases/com.plexapp.plugins.library.db");
    if(db.open())
    {
        QString sql("select media_parts.id,media_parts.media_item_id, media_parts.directory_id,file, media_item_count, title, studio, rating, rating_count, tagline, summary, quotes, content_rating, content_rating_age,tags_genre,tags_director,tags_writer,tags_star,originally_available_at, year,tags_country,audience_rating from  media_parts inner join metadata_items on media_parts.media_item_id=metadata_items.id where directory_id > 0;");
        QSqlQuery query = db.exec(sql);

        while(query.next())
        {
            QString file = query.record().field("file").value().toString();
            int id = query.record().field("id").value().toInt();
            int meda_item_id = query.record().field("media_item_id").value().toInt();
            int dirid  = query.record().field("directory_id").value().toInt();

            qDebug() << file << "-" << dirid;
        }

    }


}
//select media_parts.id,media_parts.media_item_id, media_parts.directory_id,file,media_items.* from  media_parts inner join media_items on media_parts.media_item_id=media_items.id where directory_id > 0;

DialogDeepProbe::~DialogDeepProbe()
{
    delete ui;
}
