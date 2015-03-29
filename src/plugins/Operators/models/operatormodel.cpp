#include "operatormodel.h"

#include <QtSql>

OperatorModel ::OperatorModel(QObject *parent) :
    AdvItemModel(parent),
    m_sql(0)
{
    m_location = 10032;
}

bool OperatorModel::populate()
{
    emit beginResetModel();

    if (!m_sql) {
        m_sql = new QSqlQuery();
    }

    m_sql->exec(QString("SELECT oper.co_id as id, oper.co_name as name, op_date_begin_education, "
             "op_date_attestation, op_remove_attestation, oj_name, sh_name, op_down, "
             "attest.co_name as attestator, oper.op_attestator as attestator_id "
             "FROM operators as oper "
             "JOIN operator_jobs ON oj_id = op_job "
             "JOIN contractors as attest ON attest.co_id = op_attestator "
             "JOIN shifts ON sh_id = oper.op_shift AND sh_location = oj_location "
             "WHERE oper.co_type = 10 AND sh_location = %1").arg(m_location));

    if (m_sql->lastError().isValid()) {
        qCritical() << m_sql->lastError();
        setLastError(m_sql->lastError());
        return false;
    }

    emit endResetModel();
    return true;
}

void OperatorModel::setLocation(int location)
{
    m_location = location;
}


int OperatorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_sql ? m_sql->size() : 0;
}

int OperatorModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 9;
}

QVariant OperatorModel::data(const QModelIndex &index, int role) const
{
    if (!m_sql || !index.isValid()) {
        return QVariant();
    }


    if (!m_sql->seek(index.row())) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {;
        case IdCol: return m_sql->value("id").toInt();
        case DownCol: return m_sql->value("op_down").toInt();
        case NameCol: return m_sql->value("name").toString();
        case BeginEducCol: return dateToString(m_sql->value("op_date_begin_education").toDate());
        case DateAttestCol: return dateToString(m_sql->value("op_date_attestation").toDate());
        case DateRemoveCol: return dateToString(m_sql->value("op_remove_attestation").toDate());
        case JobCol: return m_sql->value("oj_name").toString();
        case ShiftCol: return m_sql->value("sh_name").toString();
        case AttestatorCol: return m_sql->value("attestator").toString();
        }
    }

    return AdvItemModel::data(index, role);
}

QVariant OperatorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {
            case IdCol: return tr("Id");
            case DownCol: return tr("Down");
            case NameCol: return tr("Name");
            case BeginEducCol: return tr("Education");
            case DateAttestCol: return tr("Attestation");
            case DateRemoveCol: return tr("Remove");
            case JobCol: return tr("Job");
            case ShiftCol: return tr("Shift");
            case AttestatorCol: return tr("Attestator");
            }
        }
        if (role == Qt::TextAlignmentRole) {
            switch(section) {
            case IdCol: return Qt::AlignCenter;
            case DownCol: return Qt::AlignCenter;
            case NameCol: return Qt::AlignLeft + Qt::AlignVCenter;
            case BeginEducCol: return Qt::AlignCenter;
            case DateAttestCol: return Qt::AlignCenter;
            case DateRemoveCol: return Qt::AlignCenter;
            case JobCol: return Qt::AlignLeft + Qt::AlignVCenter;
            case ShiftCol: return Qt::AlignLeft + Qt::AlignVCenter;
            case AttestatorCol: return Qt::AlignLeft + Qt::AlignVCenter;
            }
        }
    }

    return AdvItemModel::headerData(section, orientation, role);
}

bool OperatorModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return true;
}

QString OperatorModel::dateToString(const QDate &date) const
{
    return date.toString("dd.MM.yyyy");
}

