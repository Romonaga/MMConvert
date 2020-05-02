#include "filefindsimpledelegate.h"
#include <QStringList>

FileFindSimpleDelegate::FileFindSimpleDelegate(bool dirOnly, QObject *pobj) : _dirOnly(dirOnly), QStyledItemDelegate(pobj)
{

}

QWidget *FileFindSimpleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QFileDialog* fileDialog = new QFileDialog(parent);

    if(_dirOnly)
    {
        fileDialog->setFileMode(QFileDialog::Directory);
        fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    }

    return fileDialog;
}

void FileFindSimpleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QFileDialog* fileDialog = qobject_cast<QFileDialog*>(editor);
   fileDialog->setDirectory(index.model()->data(index, Qt::EditRole).toString());
}

void FileFindSimpleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

   QFileDialog* fileDialog = qobject_cast<QFileDialog*>(editor);
   if(fileDialog->result())
       model->setData(index, fileDialog->selectedFiles()[0], Qt::EditRole);
}
