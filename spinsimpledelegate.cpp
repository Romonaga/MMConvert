#include "spinsimpledelegate.h"
#include <QSpinBox>

SpinSimpleDelegate::SpinSimpleDelegate(int min, int max, QObject *pobj) : _min(min), _max(max), QStyledItemDelegate(pobj)
{

}

QWidget *SpinSimpleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox* spinBox = new QSpinBox(parent);
    spinBox->setMinimum(_min);
    spinBox->setMaximum(_max);

    return spinBox;

}

void SpinSimpleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void SpinSimpleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}
