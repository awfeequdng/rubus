#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include <QString>
#include <QObject>

class Contractor
{
public:
    enum Type {
        Organization = 0,
        Employee = 1,
        Maintainer = 2
    };

    Contractor();

    static QString nameByType(int type);
};

#endif // CONTRACTOR_H
