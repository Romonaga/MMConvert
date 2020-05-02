#include "plexmediatablemodel.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QColor>

PlexMediaTableModel::PlexMediaTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

QString PlexMediaTableModel::selectStatement() const
{
    return QString("select media_parts.id,media_parts.media_item_id, media_parts.directory_id,media_parts.file,metadata_items.title,metadata_items.year, metadata_items.tags_genre, metadata_items.studio,metadata_items.rating, metadata_items.tagline, metadata_items.summary, metadata_items.content_rating,metadata_items.tags_director, metadata_items.tags_writer, metadata_items.tags_star, metadata_items.originally_available_at, metadata_items.tags_country, metadata_items.audience_rating from  media_parts inner join media_items on media_parts.media_item_id = media_items.id inner join metadata_items on media_items.metadata_item_id = metadata_items.id where directory_id > 0");

}

QVariant PlexMediaTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        if(getHideColumn(section) == false)
            return getColumnName(section);

    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
        return QSqlTableModel::headerData(section, orientation, role);


    return QVariant();
}

Qt::ItemFlags PlexMediaTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::NoItemFlags;

    if(getColumnEnable(index.column()))
        flags |= Qt::ItemIsEnabled;

    if(getColumnFmtType(index.column()) == ChkBox)
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsUserTristate;

    if(getColumnEdit(index.column()))
        flags |= Qt::ItemIsEditable;

    if(getColumnSelectable(index.column()))
        flags |= Qt::ItemIsSelectable;


    return flags;
}

QVariant PlexMediaTableModel::data(const QModelIndex &index, int role) const
{

    if(Qt::TextColorRole == role)
    {
        switch(index.column())
        {
            case tags_genre:
                return QVariant::fromValue(QColor(0,0,255));
            case studio:
                return QVariant::fromValue(QColor(0,125,255));
            case year:
                return QVariant::fromValue(QColor(0,113,230));
        }
    }

   return QSqlTableModel::data(index, role);
}

const char *PlexMediaTableModel::getColumnName(int index)
{
    const char* columnNames[] = {PLEX_INFO(PLEX_INFO_COLNAME)};
    return columnNames[index];
}

bool PlexMediaTableModel::getHideColumn(int index)
{
    const bool columnHide[] = {PLEX_INFO(PLEX_INFO_HIDE)};
    return columnHide[index];
}


bool PlexMediaTableModel::getColumnEdit(int index)
{
    const bool columnEdit[] = {PLEX_INFO(PLEX_INFO_EDIT)};
    return columnEdit[index];
}

bool PlexMediaTableModel::getColumnEnable(int index)
{
    const bool columnEnable[] = {PLEX_INFO(PLEX_INFO_ENABLE)};
    return columnEnable[index];
}

bool PlexMediaTableModel::getColumnSelectable(int index)
{
    const bool columnSelectable[] = {PLEX_INFO(PLEX_INFO_SELECTABLE)};
    return columnSelectable[index];
}

FmtType PlexMediaTableModel::getColumnFmtType(int index)
{
    const FmtType colmnFmtType[] = {PLEX_INFO(PLEX_INFO_FMTTYPE)};
    return colmnFmtType[index];
}

bool PlexMediaTableModel::getColumnPopup(int index)
{
    const bool columnPopUp[] = {PLEX_INFO(PLEX_INFO_POPUP)};
    return columnPopUp[index];
}

QMap<QString,QString> PlexMediaTableModel::getFileGenre()
{
    QMap<QString,QString> list;
    for(int counter = 0; counter < rowCount(); counter++)
    {
        QString genre = record(counter).value(tags_genre).toString().split('|')[0];
        QString rfile = record(counter).value(file).toString();

        genre = (genre.length() > 0) ? genre : "Drama";
        list.insert(rfile, genre);
    }
    return list;
}

QStringList PlexMediaTableModel::getParentGenre()
{
    QStringList list;
    for(int counter = 0; counter < rowCount(); counter++)
    {
        QString genre = record(counter).value(tags_genre).toString().split('|')[0];
        genre = (genre.length() > 0) ? genre : "Drama";
        if(list.contains(genre) == false)
            list.append(genre);
    }
    return list;
}


//"select media_parts.id,media_parts.media_item_id, media_parts.directory_id,media_parts.file,metadata_items.title, metadata_items.studio,metadata_items.rating, metadata_items.tagline, metadata_items.summary, metadata_items.content_rating,metadata_items.tags_genre, metadata_items.tags_director, metadata_items.tags_writer, metadata_items.tags_star, metadata_items.originally_available_at, metadata_items.year,metadata_items.tags_country, metadata_items.audience_rating from  media_parts inner join media_items on media_parts.media_item_id = media_items.id inner join metadata_items on media_items.metadata_item_id = metadata_items.id where directory_id > 0"


/*
select media_parts.id,media_parts.media_item_id, media_parts.directory_id,media_parts.file,metadata_items.title, metadata_items.studio,metadata_items.rating, metadata_items.tagline, metadata_items.summary,
metadata_items.content_rating,metadata_items.tags_genre, metadata_items.tags_director, metadata_items.tags_writer, metadata_items.tags_star,
metadata_items.originally_available_at, metadata_items.year,metadata_items.tags_country,
metadata_items.audience_rating
from  media_parts
inner join media_items on media_parts.media_item_id = media_items.id
inner join metadata_items on media_items.metadata_item_id = metadata_items.id where directory_id > 0
*/

