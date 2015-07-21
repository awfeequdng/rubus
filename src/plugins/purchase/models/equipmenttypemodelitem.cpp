#include "equipmenttypemodelitem.h"
#include "qdebug.h"

EquipmentTypeModelItem::EquipmentTypeModelItem() :
    QObject(0),
    m_id(-1),
    m_parentId(-1),
    m_parent(0),
    m_storageId(-1),
    m_checked(false),
    m_used(false)
{

}

EquipmentTypeModelItem::EquipmentTypeModelItem(int id, QString name, int parentId, EquipmentTypeModelItem *parent) :
    QObject(parent),
    m_id(id),
    m_parentId(parentId),
    m_name(name),
    m_parent(parent),
    m_storageId(-1),
    m_checked(false),
    m_used(false)
{
    if (parent) {
        parent->addChild(this);
    }
}

EquipmentTypeModelItem::~EquipmentTypeModelItem()
{
    m_parent = 0;
    clearChildrenItems();
}

void EquipmentTypeModelItem::idWithChildrenIndexes(QString &indexes) const
{
    if (!indexes.isEmpty()) {
        indexes.append(',');
    }

    indexes.append(QString::number(id()));

    for (int i = 0; i < childCount(); i++) {
        child(i)->idWithChildrenIndexes(indexes);
    }
}

void EquipmentTypeModelItem::setUsed(bool used)
{
    if (used && m_parent) {
        m_parent->setUsed(true);
    }

    m_used = used;
}

void EquipmentTypeModelItem::setParent(EquipmentTypeModelItem *parent)
{
    m_parent = parent;
    parent->addChild(this);
}

void EquipmentTypeModelItem::clearChildrenItems()
{

    m_children.clear();
}

EquipmentTypeModelItem *EquipmentTypeModelItem::child(int i) const
{
    return i < childCount() ? m_children.at(i) : 0;
}

int EquipmentTypeModelItem::at() const
{
    if (m_parent)
        return m_parent->children().indexOf(const_cast<EquipmentTypeModelItem*>(this));

    return 0;
}
