#ifndef VISITSMODEL_H
#define VISITSMODEL_H

#include <QAbstractTableModel>
#include "visit.h"
#include "databasemanager.h"

class VisitsModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    explicit VisitsModel(DatabaseManager * dbManager, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
 private:
    static QString COL_NAMES [Visit::MAX_TYPES + 1];
    DatabaseManager * dbManager;
    QMap<QDate,Visit> visits;

signals:

public slots:

};

#endif // VISITSMODEL_H
