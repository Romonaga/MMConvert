
#include "tablemodel.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <QModelIndex>
#include <QLocale>
#include <QColor>
#include <QBrush>


TableModel::TableModel(QObject *parent,  QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}



void TableModel::addDB(MMConversionInfo *mmInfo)
{
    if(mmInfo->dbRecId() == -1)
    {
        QSqlRecord newRecord = record();

        newRecord.setValue(fileName, mmInfo->fileName());
        newRecord.setValue(filePath, mmInfo->getFilePath());
        newRecord.setValue(outputDir, mmInfo->OutDir());
        newRecord.setValue(formatDuration, (qulonglong) mmInfo->getFormatDuration());
        newRecord.setValue(container, mmInfo->container());
        newRecord.setValue(wasProcessed, 0);
        newRecord.setValue(wasRemoved, 0);
        newRecord.setValue(secProcessing, 0);
        newRecord.setValue(wasDeleted, mmInfo->deleteSource());
        newRecord.setValue(numStreams, mmInfo->numStreams());
        newRecord.setValue(ffmpgCmd, mmInfo->ffmpgCmd());
        newRecord.setValue(useCustomSettings, mmInfo->useCustomSettings());
        newRecord.setValue(preset, mmInfo->preset());
        newRecord.setValue(crf, mmInfo->crf());
        newRecord.setValue(stripSubs, mmInfo->stripSubs());
        newRecord.setValue(streamsInfo, mmInfo->getStreamsMetaInfo());
        newRecord.setValue(formatInfo, mmInfo->getFormatMetaInfo());
        newRecord.setValue(createdTimeStamp, QDateTime());
        newRecord.setValue(updateTimeStamp, QDateTime());
        newRecord.setValue(conversionError, mmInfo->conversionError());
        newRecord.setValue(conversionErrorCode, mmInfo->conversionErrorCode());
        newRecord.setValue(process, mmInfo->getProcess());
        newRecord.setValue(startFileSize, (qlonglong) mmInfo->getStartFileSize());
        newRecord.setValue(endFileSize, (qlonglong)  mmInfo->getEndFileSize());

        if(insertRecord(this->rowCount(), newRecord) == false)
        {
            //Todo:: Do Something
        }

        QModelIndex startOfRow = this->index(this->rowCount(), 1);
        QModelIndex endOfRow   = this->index(this->rowCount(), this->columnCount());

        emit dataChanged(startOfRow, endOfRow);
    }
}



void TableModel::updateDB(MMConversionInfo *mmInfo)
{
    int64_t dbId = 0;
    bool deleteFile = false;
    for(int counter = 0; counter < this->rowCount(); counter++)
    {
        QSqlRecord rec = this->record(counter);
        dbId = QVariant(rec.value(idmmqueue)).toLongLong();
        deleteFile = rec.value(idmmqueue).toInt();
        mmInfo->setDeleteSource(deleteFile);

        if(dbId == mmInfo->dbRecId())
        {

            setData(index(counter, fileName), mmInfo->getFileName(), Qt::EditRole);
            setData(index(counter, filePath), mmInfo->getFilePath(), Qt::EditRole);
            setData(index(counter, outputDir), mmInfo->OutDir(), Qt::EditRole);
            setData(index(counter, process), mmInfo->getProcess(), Qt::EditRole);
            setData(index(counter, wasRemoved), mmInfo->isRemoved(), Qt::EditRole);
            setData(index(counter, wasDeleted), mmInfo->deleteSource(), Qt::EditRole);
            setData(index(counter, formatDuration), QVariant((qulonglong) mmInfo->getFormatDuration()), Qt::EditRole);
            setData(index(counter, container), mmInfo->container(), Qt::EditRole);
            setData(index(counter, wasProcessed), mmInfo->isProcessed(), Qt::EditRole);
            setData(index(counter, numStreams), mmInfo->numStreams(), Qt::EditRole);
            setData(index(counter, ffmpgCmd), mmInfo->ffmpgCmd(), Qt::EditRole);
            setData(index(counter, useCustomSettings), mmInfo->useCustomSettings(), Qt::EditRole);
            setData(index(counter, preset), mmInfo->preset(), Qt::EditRole);
            setData(index(counter, crf), mmInfo->crf(), Qt::EditRole);
            setData(index(counter, stripSubs), mmInfo->stripSubs(), Qt::EditRole);
            setData(index(counter, streamsInfo), mmInfo->getStreamsMetaInfo(), Qt::EditRole);
            setData(index(counter, formatInfo), mmInfo->getFormatMetaInfo(), Qt::EditRole);
            setData(index(counter, conversionError), mmInfo->conversionError(), Qt::EditRole);
            setData(index(counter, conversionErrorCode), mmInfo->conversionErrorCode(), Qt::EditRole);
            setData(index(counter, startFileSize), QVariant((qlonglong) mmInfo->getStartFileSize()), Qt::EditRole);
            setData(index(counter, endFileSize), QVariant((qlonglong) mmInfo->getEndFileSize()), Qt::EditRole);

            QModelIndex startOfRow = this->index(counter, 1);
            QModelIndex endOfRow   = this->index(counter, this->columnCount());

            emit dataChanged(startOfRow, endOfRow);

            return;
        }

    }
}


void TableModel::updateColumn(int row, int column, const QVariant& value)
{
    QModelIndex updIndex = this->index(row, column);
    QSqlRecord rec = this->record(row);

    rec.setValue(column, value);
    updateRowInTable(row, rec);

    setData(index(row, column), value, Qt::EditRole);
    emit dataChanged(updIndex, updIndex);
    select();
}


bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if ( (role == Qt::CheckStateRole && getColumnFmtType(index.column()) == ChkBox) ||
            (role == Qt::EditRole && getColumnFmtType(index.column()) == Bool) )
    {
        QVariant chkVal;

        chkVal = (value == 0) ? false : true;
        if(index.column() == wasRemoved && chkVal == true)
        {
            QSqlTableModel::setData(index, chkVal);
            return select();
        }
        return QSqlTableModel::setData(index, chkVal);
    }
    else
      return QSqlTableModel::setData(index, value, role);

}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (role == Qt::CheckStateRole && getColumnFmtType(index.column()) == ChkBox)
    {
        value = QSqlTableModel::data(index, Qt::EditRole);
        return (value == 0) ? Qt::Unchecked  : Qt::Checked;
    }

    if(index.column() == fileName && Qt::TextColorRole == role)
    {
        return QVariant::fromValue(QColor(0,0,255));
    }

    if(getColumnFmtType(index.column()) == File && Qt::TextColorRole == role)
    {
        value = QSqlTableModel::data(index, role);

        QString outDir = QSqlTableModel::data(index, Qt::DisplayRole).toString();
        QString fileDir = QSqlTableModel::data(index.sibling(index.row(), filePath), Qt::DisplayRole).toString();
        if(fileDir == outDir)
            return QVariant::fromValue(QColor(Qt::red));
        else
            return QVariant::fromValue(QColor(Qt::black));

    }


    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(getColumnFmtType(index.column()) == ChkBox)
        {
            value = QSqlTableModel::data(index, role);
            return (value == 1) ? true: false;
        }

        switch(getColumnFmtType(index.column()))
        {

        case FmtType::None:
            return QSqlTableModel::data(index, role);

        case FmtType::Bool:
            value = QSqlTableModel::data(index, role);
            return (value == 1) ? true : false;

        //Convert for display, keep Data for Edit.
        case FmtType::String:
            if(Qt::DisplayRole == role)
                return QLocale(QLocale::English).toString(QSqlTableModel::data(index, Qt::EditRole).toLongLong());
            else
                return QSqlTableModel::data(index, Qt::EditRole);

        //Convert for display, keep Data for Edit.
        case FmtType::Duration:
            if(Qt::DisplayRole == role)
            {
                QString durToTime;

                uint64_t duration = QSqlTableModel::data(index, role).toLongLong() / 1000;
                double hours = duration * 0.000277777777778; // 1 / 3600 (sec in 1 hour)
                double min  = (hours - (int)hours) * 60;
                double sec =  (min - (int) min) * 60;

                durToTime = QString::number((int)hours).rightJustified(2,'0') +
                        ":" + QString::number((int)min).rightJustified(2,'0') +
                        ":" + QString::number(sec).rightJustified(2,'0');

                return durToTime;
            }
            else
                return QSqlTableModel::data(index, Qt::EditRole);


        default:
            return QSqlTableModel::data(index, role);
        }

    }

    return QVariant();
}


void TableModel::updateData(MMConversionInfo *mmInfo)
{
    if(mmInfo->dbRecId() != -1)
        updateDB(mmInfo);
    else
        addDB(mmInfo);

}

const char *TableModel::getColumnName(int index)
{
    const char* columnNames[] = {COLUMN_INFO(COLUMN_INFO_COLNAME)};
    return columnNames[index];
}

bool TableModel::getHideColumn(int index)
{
    const bool columnHide[] = {COLUMN_INFO(COLUMN_INFO_HIDE)};
    return columnHide[index];
}


bool TableModel::getColumnEdit(int index)
{
    const bool columnEdit[] = {COLUMN_INFO(COLUMN_INFO_EDIT)};
    return columnEdit[index];
}

bool TableModel::getColumnEnable(int index)
{
    const bool columnEnable[] = {COLUMN_INFO(COLUMN_INFO_ENABLE)};
    return columnEnable[index];
}

bool TableModel::getColumnSelectable(int index)
{
    const bool columnSelectable[] = {COLUMN_INFO(COLUMN_INFO_SELECTABLE)};
    return columnSelectable[index];
}

FmtType TableModel::getColumnFmtType(int index)
{
    const FmtType colmnFmtType[] = {COLUMN_INFO(COLUMN_INFO_FMTTYPE)};
    return colmnFmtType[index];
}

bool TableModel::getColumnPopUp(int index)
{
    const bool columnPopup[] = {COLUMN_INFO(COLUMN_INFO_POPUP)};
    return columnPopup[index];
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        if(getHideColumn(section) == false)
            return getColumnName(section);

    return QVariant();
}


Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
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





