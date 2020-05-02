#pragma once

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QtSql/QSqlTableModel>
#include <QList>
#include "mmconversioninfo.h"

#include "mmconvertcommon.h"


class TableModel : public QSqlTableModel
{
    Q_OBJECT

public:


    explicit TableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QList<int> showColumns() const;
    void updateData(MMConversionInfo *mmInfo);
    void updateColumn(int row, int column, const QVariant& value);

    static const char *getColumnName(int index);
    static bool getHideColumn(int index);
    static bool getColumnEdit(int index);
    static bool getColumnEnable(int index);
    static bool getColumnSelectable(int index);
    static FmtType getColumnFmtType(int index);
    static bool getColumnPopUp(int index);


public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void updateDB(MMConversionInfo *mmInfo);
    void addDB(MMConversionInfo *mmInfo);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
};




#endif // TABLEMODEL_H
