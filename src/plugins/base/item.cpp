#include "item.h"

#include <QObject>
#include <QDebug>

Item::Item()
{
}

QString Item::nameType(Item::Types type)
{
    switch(type) {
    case Manufacture : return QObject::tr("Manufacture");
    case Purchase : return QObject::tr("Purchase");
    case Sparepart : return QObject::tr("Sparepart");
    }

    return QObject::tr("Unknown");
}

QString Item::idType(Item::Types type)
{
    switch(type) {
    case Manufacture : return QLatin1String("M");
    case Purchase : return QLatin1String("P");
    case Sparepart : return QLatin1String("S");
    }

    return QLatin1String("");
}

QMap<QString, QString> Item::itemMap()
{
    QMap<QString, QString> m;

    m.insert(idType(Manufacture), nameType(Manufacture));
    m.insert(idType(Purchase), nameType(Purchase));
    m.insert(idType(Sparepart), nameType(Sparepart));

    return m;
}
