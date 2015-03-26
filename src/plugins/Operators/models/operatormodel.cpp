#include "operatormodel.h"

#include <QtSql>

OperatorModel ::OperatorModel(QObject *parent) :
    AdvItemModel(parent)
{
}

bool OperatorModel::populate()
{
    emit beginResetModel();

    m_removedIds.clear();
    qDeleteAll(m_items);
    m_items.clear();

    QSqlQuery sql;
    sql.exec("SELECT FROM _table_");

    if (sql.lastError().isValid()) {
        qCritical() << sql.lastError();
        setLastError(sql.lastError());
        return false;
    }

    while(sql.next()) {
        Internal::Item *item = new Internal::Item;
        item->id = sql.value(0).toInt();
        item->name = sql.value(1).toString();

        m_items.append(item);
    }

    emit endResetModel();
    return true;
}


int OperatorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int OperatorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant OperatorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Internal::Item *item = m_items[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {;
        case IdCol : return item->id;
        case NameCol : return item->name;
        }
    }

    return AdvItemModel::data(index, role);
}

QVariant OperatorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {;
            case IdCol : return tr("Id");
            case NameCol : return tr("Name");
            }
        }
        if (role == Qt::TextAlignmentRole) {
            switch(section) {;
            case IdCol : return Qt::AlignCenter;
            case NameCol : return Qt::AlignLeft + Qt::AlignVCenter;
            }
        }
    }

    return AdvItemModel::headerData(section, orientation, role);
}

bool OperatorModel::removeRows(int row, int count, const QModelIndex &parent)
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

bool OperatorModel::submit()
{
    if (!m_removedIds.isEmpty()) {
        QSqlQuery sql;
        sql.exec(QString("DELETE FROM _table_ WHERE _id IN (%1)").arg(m_removedIds));

        if (sql.lastError().isValid()) {
            setLastError(sql.lastError());
            qCritical() << sql.lastError();
            return false;
        }

        m_removedIds.clear();

    }

    return true;
}

