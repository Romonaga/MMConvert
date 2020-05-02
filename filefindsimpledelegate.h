#ifndef FILEFINDSIMPLEDELEGATE_H
#define FILEFINDSIMPLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QFileDialog>

#include <QObject>
#include <QWidget>


class FileFindSimpleDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    FileFindSimpleDelegate(bool dirOnly, QObject* pobj = 0);

private:
    QString _fileDialogResult;
    bool _dirOnly;

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // FILEFINDSIMPLEDELEGATE_H
