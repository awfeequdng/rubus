#ifndef EQUIPMENTTYPECHECKMODEL_H
#define EQUIPMENTTYPECHECKMODEL_H

#include <QAbstractItemModel>


#include "spareparts_global.h"

class EquipmentTypeModelItem;

class SPAREPARTS_EXPORT EquipmentTypeCheckModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EquipmentTypeCheckModel(QObject *parent = 0);

    void populate(int itemId);
    EquipmentTypeModelItem *itemByIndex(QModelIndex index) const;

    void setItemId(int id);

signals:

public slots:


    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public slots:
    bool submit();

private:
    EquipmentTypeModelItem *m_rootItem;
    QHash<int, EquipmentTypeModelItem *> m_itemById;
    int m_itemId;

};

#endif // EQUIPMENTTYPECHECKMODEL_H
