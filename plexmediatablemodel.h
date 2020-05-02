#ifndef PLEXMEDIATABLEMODEL_H
#define PLEXMEDIATABLEMODEL_H

#include <QSqlDatabase>
#include <QSqlTableModel>

#include "mmconvertcommon.h"
#include <QStringList>
#include <QMap>



class PlexMediaTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit PlexMediaTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    static const char *getColumnName(int index);
    static bool getHideColumn(int index);
    static bool getColumnEdit(int index);
    static bool getColumnEnable(int index);
    static bool getColumnSelectable(int index);
    static FmtType getColumnFmtType(int index);
    static bool getColumnPopup(int index);
    QStringList getParentGenre();

    QMap<QString,QString> getFileGenre();

private:

    // QSqlTableModel interface
protected:
    QString selectStatement() const;


    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // QAbstractItemModel interface
public:
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // PLEXMEDIATABLEMODEL_H
