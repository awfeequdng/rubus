#include "equipmenttypemodel.h"
#include "equipmenttypemodelitem.h"

#include <QtSql>
#include <QIcon>
#include <QSet>

EquipmentTypeModel::EquipmentTypeModel(QObject *parent) :
    QAbstractItemModel(parent),
     m_readOnly(true)
{
    m_rootItem = new EquipmentTypeModelItem();
    m_rootItem->setUsed(true);

    m_itemIcon = QIcon(":/icons/folder.png");
}

void EquipmentTypeModel::populate()
{
    emit beginResetModel();

    qDeleteAll(m_itemById);
    m_itemById.clear();
    m_rootItem->clearChildrenItems();

    QSqlQuery sql;
    sql.exec("SELECT et_id,et_name,et_parent FROM equipment_types");

    if (sql.lastError().isValid()) {
        qDebug() << sql.lastError();
        return;
    }

    while (sql.next()) {
        int id = sql.value(0).toInt();
        EquipmentTypeModelItem *item =
                new EquipmentTypeModelItem(id,
                                           sql.value(1).toString(),
                                           sql.value(2).toInt());
        m_itemById.insert(item->id(), item);
    }

    QHashIterator<int, EquipmentTypeModelItem*> iter(m_itemById);
    while (iter.hasNext()) {
        iter.next();
        EquipmentTypeModelItem *item = iter.value();

        if (item->id() == item->parentId()) {
            item->setParent(m_rootItem);
            continue;
        }

        EquipmentTypeModelItem *parent = m_itemById.value(item->parentId());
        item->setParent(parent ? parent : m_rootItem);
    }

    emit endResetModel();
}

EquipmentTypeModelItem *EquipmentTypeModel::itemByIndex(QModelIndex index) const
{
    return static_cast<EquipmentTypeModelItem*>(index.internalPointer());
}

QModelIndex EquipmentTypeModel::index(int row, int column, const QModelIndex &parent) const
{
    EquipmentTypeModelItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<EquipmentTypeModelItem*>(parent.internalPointer());

    if (!parentItem) {
        return QModelIndex();
    }

    EquipmentTypeModelItem *childItem = parentItem->child(row);

    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex EquipmentTypeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    EquipmentTypeModelItem *childItem = static_cast<EquipmentTypeModelItem*>(child.internalPointer());
    EquipmentTypeModelItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();


    return createIndex(parentItem->at(), 0, parentItem);
}

int EquipmentTypeModel::rowCount(const QModelIndex &parent) const
{
    EquipmentTypeModelItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<EquipmentTypeModelItem *>(parent.internalPointer());

    if (parentItem) {
        return parentItem->childCount();
    } else {
        return 0;
    }
}

int EquipmentTypeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant EquipmentTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    EquipmentTypeModelItem *item = static_cast<EquipmentTypeModelItem *>(index.internalPointer());

    if (!item) {
        return QVariant();
    }


    if (role == Qt::DisplayRole && index.column() == 0) {
        return item->name();
    }

    if (role == Qt::EditRole && index.column() == 0) {
        return item->id();
    }

    if (role == Qt::DecorationRole && index.column() == 0) {
        return m_itemIcon;
    }

    return QVariant();
}


Qt::ItemFlags EquipmentTypeModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

QModelIndex EquipmentTypeModel::indexById(int id) const
{
    QModelIndexList items = match(
                index(0, 0),
                Qt::EditRole,
                QVariant::fromValue(id),
                2, // look *
                Qt::MatchRecursive | Qt::MatchFixedString);


    return items.count() > 0 ? items.at(0) : QModelIndex();
}
