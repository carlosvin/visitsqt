#ifndef VISIT_H
#define VISIT_H

#include <QtCore>
#include <QVariant>

enum VisitType { MEN = 0, WOMEN = 1, BOYS = 2, GIRLS = 3 };

class Visit
{
public:

    const static int MAX_TYPES = 4;

    Visit();
    Visit(const Visit & v);
    Visit & operator=(const Visit & v);
    bool operator==(const Visit & v);
    bool operator!=(const Visit & v);
    QVariant getByColumn(int col);
    void setByColumn(int col, const QVariant &value);

    QDate getDate() const;
    int getVisit(VisitType visitType) const;

    void setDate(const QDate & inputDate);
    void setVisit(VisitType visitType, int number);

private:
    QDate date;
    int visits [MAX_TYPES];
};

#endif // VISIT_H
