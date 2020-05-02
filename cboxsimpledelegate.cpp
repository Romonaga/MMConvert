#include "cboxsimpledelegate.h"

CBoxSimpleDelegate::CBoxSimpleDelegate(const QStringList &values, QObject* pobj) : _values(values), QStyledItemDelegate(pobj)
{

}

QWidget *CBoxSimpleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   QComboBox* comboBox = new QComboBox(parent);
   comboBox->addItems(_values);

   return comboBox;
}

void CBoxSimpleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
   QString value = comboBox->currentText();
   model->setData(index, value, Qt::EditRole);
}

void CBoxSimpleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    //qDebug() << "Value:" << value;
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(comboBox->findText(value));
}
