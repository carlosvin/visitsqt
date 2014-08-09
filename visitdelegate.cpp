#include "visitdelegate.h"
#include <QDateEdit>
#include <QSpinBox>

VisitDelegate::VisitDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *VisitDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index ) const
{
    if (index.column() == 0){
        return new QDateEdit(parent);
    }else{
        QSpinBox *editor = new QSpinBox(parent);
        editor->setMinimum(0);
        return editor;
    }
}

void VisitDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if (index.column() == 0){
        QDate value = index.model()->data(index, Qt::EditRole).toDate();
        QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
        dateEdit->setDate(value);
    } else {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
}

void VisitDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        model->setData(index, static_cast<QDateEdit*>(editor)->date(), Qt::EditRole);
    }
    else
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        model->setData(index, spinBox->value(), Qt::EditRole);
    }
}

void VisitDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
