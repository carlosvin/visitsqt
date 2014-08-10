#include "visitsmodel.h"

QString VisitsModel::COL_NAMES[Visit::MAX_TYPES + 1] = {"Fecha", "Hombres", "Mujeres", "Ninos", "Ninas" };

VisitsModel::VisitsModel(DatabaseManager *  dbManager, QObject *parent) :
    QAbstractTableModel(parent),
    dbManager(dbManager)
{
    this->dbManager = dbManager;
    QList<Visit> visits;
    dbManager->loadVisits(visits);
    foreach (Visit visit, visits) {
        this->visits[visit.getDate()] = visit;
    }
}

int VisitsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return visits.count();
}

int VisitsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return Visit::MAX_TYPES + 1;
}

QVariant VisitsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const int row = index.row();

        if (row >= visits.count() || row < 0)
            return QVariant();

        QDate key = visits.keys()[row];
        const int col = index.column();
        if (col == 0)
        {
            return QVariant(visits[key].getDate());
        }
        else
        {
            return QVariant(visits[key].getVisit(static_cast<VisitType>(col - 1)));
        }
    }
    else
    {
        return QVariant();
    }
}

QVariant VisitsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section >= 0 && section < (Visit::MAX_TYPES+1)){
            return QVariant(VisitsModel::COL_NAMES [section]);
        }else{
            return QVariant();
        }
    }else{
        return QVariant();
    }
}

bool VisitsModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    Visit v;
    if (rows > 0 && !visits.contains(v.getDate()))
    {
        beginInsertRows(QModelIndex(), position, position);
        visits[v.getDate()]= v;
        endInsertRows();
        return true;
    }
    else
    {
        qDebug()<< "Not updated " << position <<  " , " << rows ;
        return false;
    }
}

bool VisitsModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(index, position, position+rows-1);

    for (int row=position; row < rows; row++) {
        QDate k = visits.keys()[row];
        if(dbManager->deleteVisit(k)){
            visits.remove(k);
        }else{
            qDebug() << "deleted " << k;
        }
    }

    endRemoveRows();
    return true;
}

bool VisitsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int col = index.column();
        if (col==0){
            QDate newDate = value.value<QDate>();
            QDate oldDate = visits.keys()[index.row()];
            if (newDate != oldDate && !visits.contains(newDate))
            {
                Visit v = visits[oldDate];
                v.setDate(newDate);
                visits.remove(oldDate);
                visits[newDate] = v;
                dbManager->deleteVisit(oldDate);
                if (!dbManager->insertVisit(v))
                {
                    qDebug() << dbManager->lastError();
                }
            }
        }
        else
        {
            visits[visits.keys()[index.row()]].setVisit(static_cast<VisitType>(col - 1), value.value<int>());
            //dbManager->updateVisit();
        }
        emit(dataChanged(index, index));
        return true;
    } else {
        return false;
    }
}

Qt::ItemFlags VisitsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
