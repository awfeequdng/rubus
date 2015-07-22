#include "equipmenttypecheckmodel.h"
#include "equipmenttypemodelitem.h"

#include <QtSql>

EquipmentTypeCheckModel::EquipmentTypeCheckModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new EquipmentTypeModelItem();
}

void EquipmentTypeCheckModel::populate(int itemId)
{
    m_itemId = itemId;

    emit beginResetModel();

    qDeleteAll(m_itemById);
    m_itemById.clear();
    m_rootItem->clearChildrenItems();


    QSqlQuery sql;
    sql.exec(QString("SELECT et_id,et_name,et_parent, "
                     "(SELECT COUNT(*) FROM items_equipment_types WHERE "
                     "ie_equipment_type = et_id AND ie_item = %1)"
                     "FROM equipment_types")
             .arg(itemId));

    if (sql.lastError().isValid()) {
        qDebug() << sql.lastError();
        return;
    }

    while (sql.next()) {
        EquipmentTypeModelItem *item =
                new EquipmentTypeModelItem(sql.value(0).toInt(),
                                           sql.value(1).toString(),
                                           sql.value(2).toInt());
        item->setChecked(sql.value(3).toInt() > 0);
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

EquipmentTypeModelItem *EquipmentTypeCheckModel::itemByIndex(QModelIndex index) const
{
    return static_cast<EquipmentTypeModelItem*>(index.internalPointer());
}

void EquipmentTypeCheckModel::setItemId(int id)
{
    m_itemId = id;
}


QModelIndex EquipmentTypeCheckModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex EquipmentTypeCheckModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    EquipmentTypeModelItem *childItem = static_cast<EquipmentTypeModelItem*>(child.internalPointer());
    EquipmentTypeModelItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();


    return createIndex(parentItem->at(), 0, parentItem);
}

int EquipmentTypeCheckModel::rowCount(const QModelIndex &parent) const
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

int EquipmentTypeCheckModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant EquipmentTypeCheckModel::data(const QModelIndex &index, int role) const
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

    if (role == Qt::CheckStateRole && index.column() == 0) {
        return item->isChecked() ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

bool EquipmentTypeCheckModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

Qt::ItemFlags EquipmentTypeCheckModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool EquipmentTypeCheckModel::setData(const QModelIndex &index, const QVariant &/*value*/, int role)
{
    if (role == Qt::CheckStateRole && index.column() == 0) {
        EquipmentTypeModelItem *item = static_cast<EquipmentTypeModelItem *>(index.internalPointer());

        if (!item) {
            return false;
        }

        item->setChecked(!item->isChecked());

        return true;
    }

    return false;
}

bool EquipmentTypeCheckModel::submit()
{
    QSqlQuery sql;

    sql.exec(QString("DELETE FROM items_equipment_types WHERE ie_item = %1").arg(m_itemId));

    if (sql.lastError().isValid()) {
        qCritical() << sql.lastError();
        return false;
    }

    EquipmentTypeModelItem *item;
    sql.prepare("INSERT INTO items_equipment_types (ie_item,ie_equipment_type) "
                "VALUES (:item,:type)");

    foreach (item, m_itemById) {
        if (item->isChecked()) {
            sql.bindValue(":item", m_itemId);
            sql.bindValue(":type", item->id());

            if (!sql.exec()) {
                qCritical() << sql.lastError();
                return false;
            }
        }
    }

    return true;
}


