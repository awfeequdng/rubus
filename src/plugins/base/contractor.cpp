#include "contractor.h"

Contractor::Contractor()
{
}

QString Contractor::nameByType(int type)
{
    switch(type) {
    case Organization : return QObject::tr("Organization");
    case Employee : return QObject::tr("Employee");
    case Maintainer : return QObject::tr("Maintainer");
    }

    return QString();
}
