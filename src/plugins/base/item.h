#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QMap>

class Item
{
public:
    enum Types {
        Manufacture,
        Purchase,
        Sparepart
    };

    Item();

    static QString nameType(Types type);
    static QString idType(Types type);
    static QMap<QString, QString> itemMap();
};

#endif // ITEM_H
