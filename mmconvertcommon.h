#ifndef MMCONVERTCOMMON_H
#define MMCONVERTCOMMON_H

enum FmtType { None, Bool, String, Duration, ChkBox, CBox, File, Spin};


            //type, name,    dbname, colName, hide, selectable, edit, enable, fmtType
#define COLUMN_INFO(X)                                                  \
            X( 0, idmmqueue, "idmmqueue", "Queue ID", false, true, false, true, None, false),                          \
            X( 1, fileName, "fileName", "File", false, false, true, false, None, true),                          \
            X( 2, filePath, "filePath", "Path", true, false, true, false, None, false),                          \
            X( 3, outputDir, "outputDir", "Out-Path", false, false, true, true, File, false),                          \
            X( 4, process, "process","Process", false, false, true, true, ChkBox, false),                          \
            X( 5, wasRemoved, "wasRemoved","Remove", false, false, true, true, Bool, false),                          \
            X( 6, wasDeleted, "wasDeleted","Delete", false, false, true, true, ChkBox, false),                          \
            X( 7, stripSubs, "stripSubs","Strip-Subs", false, false, true, true, ChkBox, false),                          \
            X( 8, useCustomSettings, "useCustomSettings","Custom", false, false, true, true, ChkBox, false),                          \
            X( 9, preset, "preset","Preset", false, false, true, true, CBox, false),                          \
            X(10, crf, "crf","Crf", false, false, true, true, Spin, false),                          \
            X(11, formatDuration, "formatDuration","Duration", false, false, true, false, Duration, false),                          \
            X(12, container, "container"," Container", false, false, true, false, None, false),                          \
            X(13, wasProcessed, "wasProcessed","", true, false, true, false, None, false),                          \
            X(14, createdTimeStamp, "createdTimeStamp","Created", false, false, false, false, None, false),                          \
            X(15, updateTimeStamp, "updateTimeStamp","", true, false, false, false, None, false),                          \
            X(16, secProcessing, "secProcessing","", true, false, true, false, None, false),                          \
            X(17, numStreams, "numStreams","#Streams", false, false, true, false, None, false),                          \
            X(18, ffmpgCmd, "ffmpgCmd","", true, false, true, false, None, false),                          \
            X(19, streamsInfo, "streamsInfo","", true, false, true, false, None, false),                          \
            X(20, formatInfo, "formatInfo","", true, false, true, false, None, false),                          \
            X(21, conversionError, "conversionError","", true, false, true, false, None, false),                          \
            X(22, conversionErrorCode, "conversionErrorCode","", true, false, true, false,None, false),                          \
            X(23, startFileSize, "startFileSize","Size", false, false, true, false, String, false),                          \
            X(24, endFileSize, "endFileSize","", true, false, true, false, None, false)                          \


#define COLUMN_INFO_ENUM(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) name = type
#define COLUMN_INFO_TYPE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) type
#define COLUMN_INFO_NAME(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) name
#define COLUMN_INFO_DBNAME(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) dbname
#define COLUMN_INFO_COLNAME(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) colName
#define COLUMN_INFO_HIDE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) hide
#define COLUMN_INFO_SELECTABLE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) selectable
#define COLUMN_INFO_EDIT(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) edit
#define COLUMN_INFO_ENABLE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) enable
#define COLUMN_INFO_FMTTYPE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) fmtType
#define COLUMN_INFO_POPUP(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) popup


typedef enum {
    COLUMN_INFO(COLUMN_INFO_ENUM),
    COLUMN_INFO_ENUM_COUNT
} ColumInfo;

//type, name,    dbname, colName, hide, selectable, edit, enable, fmtType, popup
#define PLEX_INFO(X)                                                  \
    X( 0, id, "id", "Queue ID", true, false, false, false, None, false),                          \
    X( 1, media_item_id, "media_item_id", "", true, false, false, false, None, false),                          \
    X( 2, directory_id, "directory_id", "", true, false, false, false, None, false),                          \
    X( 3, file, "file", "File", false, true, false, false, None, true),                          \
    X( 4, title, "title","Title", false, false, false, false, None, false),                          \
    X( 5, year, "year","year", false, false, false, false, None, false),                          \
    X( 6, tags_genre, "tags_genre","Genre", false, false, false, false, None, true),                          \
    X( 7, studio, "studio","Studio", false, false, false, false, None, true),                          \
    X( 8, rating, "rating","Rating", false, false, false, false, None, false),                          \
    X( 9, tagline, "tagline","Tagline", false, false, false, false, None, false),                          \
    X(10, summary, "summary","Summary", false, false, false, false, None, true),                          \
    X(11, content_rating, "content_rating","MPAR", false, false, false, false, None, false),                          \
    X(12, tags_director, "tags_director","Director", false, false, false, false, None, false),                          \
    X(13, tags_writer, "tags_writer","Writer", false, false, false, false, None, false),                          \
    X(14, tags_star, "tags_star","Star", false, false, false, false, None, false),                          \
    X(15, originally_available_at, "originally_available_at","Released", true, false, false, false, None, false),                          \
    X(16, tags_country, "tags_country","Country", true, false, false, false, None, false),                          \
    X(17, audience_rating, "audience_rating","Rating", true, false, false, false, None, false)                          \


#define PLEX_INFO_ENUM(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) name = type
#define PLEX_INFO_TYPE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) type
#define PLEX_INFO_NAME(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) name
#define PLEX_INFO_DBNAME(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) dbname
#define PLEX_INFO_COLNAME(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) colName
#define PLEX_INFO_HIDE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) hide
#define PLEX_INFO_SELECTABLE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) selectable
#define PLEX_INFO_EDIT(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) edit
#define PLEX_INFO_ENABLE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) enable
#define PLEX_INFO_FMTTYPE(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) fmtType
#define PLEX_INFO_POPUP(type, name, dbname, colName, hide, selectable, edit, enable, fmtType, popup) popup


typedef enum {
PLEX_INFO(PLEX_INFO_ENUM),
PLEX_INFO_ENUM_COUNT
} PlexInfo;


#endif // MMCONVERTCOMMON_H
