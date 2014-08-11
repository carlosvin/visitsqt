#include "visit.h"


Visit::Visit()
{
    date = QDate::currentDate();
    for (int i = 0; i<MAX_TYPES; i++)
    {
        visits[i] = 0;
    }
}

Visit::Visit(const Visit & v)
{
    *this = v;
}

Visit & Visit::operator=(const Visit & v)
{
    date = v.date;
    for (int i = 0; i<MAX_TYPES; i++)
    {
        visits[i] = v.visits[i];
    }
    return *this;
}

bool Visit::operator==(const Visit & v)
{
    for (int i = 0; i<MAX_TYPES; i++)
    {
        if (visits[i] != v.visits[i]){
            return false;
        }
    }
    return date == v.date;
}

bool Visit::operator!=(const Visit & v)
{
    return !(*this == v);
}

QDate Visit::getDate() const{
    return date;
}

void Visit::setDate(const QDate & inputDate){
    date.setDate(inputDate.year(), inputDate.month(), inputDate.day());
}

void Visit::setVisit(VisitType visitType, int number){
    visits[visitType] = number;
}

int Visit::getVisit(VisitType visitType) const{
    return visits[visitType];
}

