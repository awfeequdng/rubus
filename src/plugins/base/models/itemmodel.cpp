#include "itemmodel.h"

#include <QtSql>

using namespace Internal;

ItemModel::ItemModel(QObject *parent) :
    AdvItemModel(parent)
{
}

bool ItemModel::populate()
{
    emit beginResetModel();

    qDeleteAll(m_items);
    m_items.clear();
    m_removedIds.clear();


    QSqlQuery sql;
    sql.exec(QString("SELECT it_id, it_name, it_article, it_type, "
                     "it_unit, un_name, it_active FROM items "
                     "JOIN units ON un_id = it_unit "));

    if (sql.lastError().isValid()) {
        setLastError(sql.lastError());
        qCritical() << sql.lastError();
    }

    while (sql.next()) {
        Item *item = new Item;
        item->id = sql.value(0).toInt();
        item->name = sql.value(1).toString();
        item->article = sql.value(2).toString();
        item->typeId = sql.value(3).toInt();
        item->typeName = QString();
        item->unitId = sql.value(4).toString();
        item->unitTitle = sql.value(5).toString();
        item->active = sql.value(6).toBool();

        m_items.append(item);
    }

    emit endResetModel();
    return !sql.lastError().isValid();
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int ItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Item *item = m_items.at(index.row());

    if (role == Qt::EditRole || role == Qt::DisplayRole) {
        switch(index.column()) {
        case IdCol : return item->id;
        case NameCol : return item->name;
        case ArticleCol : return item->article;
        case TypeCol : return role == Qt::EditRole ? QString::number(item->typeId) : item->typeName;
        case UnitCol : return role == Qt::EditRole ? item->unitId : item->unitTitle;
        }
    }

    return AdvItemModel::data(index, role);
}

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {
            case IdCol : return tr("Id");
            case NameCol : return tr("Name");
            case ArticleCol : return tr("Article");
            case TypeCol : return tr("Type");
            case UnitCol : return tr("Unit");
            }
        }

        if (role == Qt::TextAlignmentRole) {
            switch(section) {
            case IdCol : return Qt::AlignCenter;
            case NameCol : return Qt::AlignVCenter + Qt::AlignLeft;
            case ArticleCol : return Qt::AlignVCenter + Qt::AlignLeft;
            case TypeCol : return Qt::AlignVCenter + Qt::AlignLeft;
            case UnitCol : return Qt::AlignVCenter + Qt::AlignLeft;
            }
        }
    }

    return AdvItemModel::headerData(section, orientation, role);
}

bool ItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    emit beginRemoveRows(parent, row, row + count -1);

    if (!m_removedIds.isEmpty()) {
        m_removedIds.append(",");
    }
    m_removedIds += QString::number(m_items[row]->id);
    m_items.removeAt(row);
    emit endRemoveRows();

    return true;
}

bool ItemModel::submit()
{
    if (!m_removedIds.isEmpty()) {
        QSqlQuery sql;
        sql.exec(QString("DELETE FROM items WHERE it_id IN (%1)").arg(m_removedIds));

        if (sql.lastError().isValid()) {
            setLastError(sql.lastError());
            qCritical() << sql.lastError();
            return false;
        }

        m_removedIds.clear();

    }

    return true;
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

