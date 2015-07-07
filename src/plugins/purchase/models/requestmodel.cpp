#include "requestmodel.h"
#include "purchaseconstants.h"

#include <QDebug>
#include <QtSql>

RequestModel::RequestModel(QObject *parent) :
    AdvItemModel(parent),
    m_sql(0)
{
}

bool RequestModel::populate()
{
    emit beginResetModel();

    if (!m_sql)  {
        m_sql = new QSqlQuery();
    }

    m_sql->exec("SELECT re_id, re_date, re_state, location, "
                "item, equipment, re_qty, re_balance,unit FROM v_requests");

    if (m_sql->lastError().isValid()) {
        setErrorString(m_sql->lastError().text());
        return false;
    }

    emit endResetModel();

    return true;
}

int RequestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_sql ? m_sql->size() : 0;
}

int RequestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 8;
}


bool RequestModel::submit()
{
    return false;
}

QVariant RequestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case IdCol : return tr("Id");
            case StateCol : return tr("State");
            case DateCol : return tr("Date");
            case StorageCol : return tr("Location");
            case EquipmentCol : return tr("Equipment");
            case ItemCol : return tr("Item");
            case QtyCol : return tr("Qty");
            case BalanceCol : return tr("Balance");
            }
        }

        if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case IdCol : return Qt::AlignCenter;
            case StateCol : return Qt::AlignCenter;
            case DateCol : return Qt::AlignCenter;
            case StorageCol : return Qt::AlignLeft + Qt::AlignVCenter;
            case EquipmentCol : return Qt::AlignLeft + Qt::AlignVCenter;
            case ItemCol : return Qt::AlignLeft + Qt::AlignVCenter;
            case QtyCol : return Qt::AlignCenter;
            case BalanceCol : return Qt::AlignCenter;
            }
        }
    }

    return AdvItemModel::headerData(section, orientation, role);
}


QVariant RequestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_sql || !m_sql->seek(index.row())) {
        return QVariant();
    }

    QSqlRecord rec = m_sql->record();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case IdCol : return rec.value("re_id").toInt();
        case StateCol : return stateName(rec.value("re_state").toInt());
        case DateCol : return rec.value("re_date").toDate();
        case StorageCol : return rec.value("location").toString();
        case EquipmentCol : return rec.value("equipment").toString();
        case ItemCol : return rec.value("item").toString();
        case QtyCol : return QLocale::system().toString(rec.value("re_qty").toDouble(), 'f', 2).append( rec.value("unit").toString());
        case BalanceCol : return QLocale::system().toString(rec.value("re_balance").toDouble(), 'f', 2).append( rec.value("unit").toString());
        }
    }

    return AdvItemModel::data(index, role);
}

QString RequestModel::stateName(int state)
{
    switch(state) {
        case Constants::STATE_HIDDEN : return tr("Hidden");
        case Constants::STATE_PUBLIC : return tr("Public");
        case Constants::STATE_ORDERED : return tr("Ordered");
        case Constants::STATE_PAID : return tr("Paid");
        case Constants::STATE_DELIVERED : return tr("Delivered");
        case Constants::STATE_RECEIVED : return tr("Received");
    default : return tr("!Err!");
    }
}
