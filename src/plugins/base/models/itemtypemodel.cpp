#include "itemtypemodel.h"
#include "item.h"

ItemTypeModel::ItemTypeModel(QObject *parent) :
    AdvItemModel(parent)
{
    m_nameById = Item::itemMap();
    m_items = m_nameById.keys();
}


int ItemTypeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int ItemTypeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ItemTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {
        case 0 : return m_items[index.row()];
        case 1 : return m_nameById.value(m_items[index.row()]);
        }
    }

    return AdvItemModel::data(index, role);
}
