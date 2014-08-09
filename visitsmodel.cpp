#include "visitsmodel.h"

QString VisitsModel::COL_NAMES[Visit::MAX_TYPES + 1] = {"Fecha", "Hombres", "Mujeres", "Ninos", "Ninas" };

VisitsModel::VisitsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


VisitsModel::VisitsModel(const QSet<Visit> & visits, QObject *parent)
    : QAbstractTableModel(parent)
{
    foreach (Visit visit, visits) {
        dates << visit.getDate().toString();
        visitsList.append(visit);
    }
}

int VisitsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return visitsList.count();
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

    const int row = index.row();

    if (row > visitsList.size() || row < 0)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        const int col = index.column();
        if (col == 0){
            return QVariant(visitsList[row].getDate());
        } else {
            return QVariant(visitsList[row].getVisit(static_cast<VisitType>(col - 1)));
        }
    } else {
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
    if (rows > 0 && !dates.contains(v.getDate().toString()))
    {
        beginInsertRows(index, position, position+1);
        dates << v.getDate().toString();
        visitsList.insert(position, v);
        qDebug() << visitsList.count() << " == " << dates.count();
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
        QDate date = visitsList[row].getDate();
        visitsList.removeAt(row);
        dates.remove(date.toString());
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
            QString newDateStr = newDate.toString();
            QString oldDateStr = visitsList[index.row()].getDate().toString();
            if (newDateStr != oldDateStr && !dates.contains(newDateStr))
            {
                dates << newDateStr;
                dates.remove(oldDateStr);
                visitsList[index.row()].setDate(newDate);

            }
        }else{
            visitsList[index.row()].setVisit(static_cast<VisitType>(col - 1), value.value<int>());
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
