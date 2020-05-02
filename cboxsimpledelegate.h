#ifndef CBOXSIMPLEDELEGATE_H
#define CBOXSIMPLEDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QObject>
#include <QStringList>


class CBoxSimpleDelegate : public QStyledItemDelegate
{
      Q_OBJECT

public:
    CBoxSimpleDelegate(const QStringList &values, QObject* pobj = 0);

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    QStringList _values;
};

#endif // CBOXSIMPLEDELEGATE_H
