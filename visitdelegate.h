#ifndef VISITDELEGATE_H
#define VISITDELEGATE_H

#include <QItemDelegate>

class VisitDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit VisitDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // VISITDELEGATE_H
