#ifndef SPINSIMPLEDELEGATE_H
#define SPINSIMPLEDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>

class SpinSimpleDelegate : public QStyledItemDelegate
{
      Q_OBJECT

public:
    SpinSimpleDelegate(int min, int max, QObject* pobj = 0);

private:
    int _min;
    int _max;

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

#endif // SPINSIMPLEDELEGATE_H
