#include "sqlmodel.h"
#include <QtSql>
#include <QDebug>

SqlModel::SqlModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_query(0)
{
}

QString SqlModel::query() const
{
    return m_query ? m_query->lastQuery() : QString();
}

void SqlModel::setQuery(const QString &query)
{
    if (!m_query) {
        m_query = new QSqlQuery();
    }

    emit beginResetModel();
    m_query->exec(query);

    if (m_query->lastError().isValid()) {
        qCritical() << m_query->lastError();
        m_errorString = m_query->lastError().text();
        emit errorStringChanged();
        return;
    }

    QSqlRecord rec = m_query->record();
    m_roles.clear();
    for (int i = 0; i < rec.count(); i++) {
        m_roles.insert(Qt::UserRole + i + 1, rec.fieldName(i).toUtf8());
    }

    emit endResetModel();
    emit queryChanged();
}

QString SqlModel::select() const
{
    return m_select;
}

void SqlModel::setSelect(const QString &select)
{
    m_select = select;
    setQuery(m_select + m_where + m_order);
}

QString SqlModel::where() const
{
    return m_where;
}

void SqlModel::setWhere(const QString &where)
{
    m_where = where;
    setQuery(m_select + m_where + m_order);
}

QString SqlModel::orderBy() const
{
    return m_order;
}

void SqlModel::setOrderBy(const QString &order)
{
    m_order = order;

    setQuery(m_select + m_where + m_order);
}

QString SqlModel::primaryKeyRole() const
{
    return m_pkeyRole;
}

void SqlModel::setPrimaryKeyRole(const QString &column)
{
    m_pkeyRole = column;
}

QString SqlModel::tableName() const
{
    return m_tableName;
}

void SqlModel::setTableName(const QString &name)
{
    m_tableName = name;
}

QString SqlModel::errorString() const
{
    return m_errorString;
}

QVariant SqlModel::value(int row, int role) const
{
    if (!m_query->seek(row)) {
        qDebug() << "can't seek in data()";
        return QVariant();
    }

    QSqlRecord rec = m_query->record();
    QVariant value;

    if (role < Qt::UserRole) {
        value = QVariant();
    } else {
        value = rec.value(role - Qt::UserRole - 1);
    }

    return value;
}

QVariant SqlModel::value(int row, QString rolename) const
{
    return value(row, m_roles.key(rolename.toUtf8()));
}

QVariant SqlModel::primaryKeyValue(int row) const
{
    return m_pkeyRole.isEmpty() ?
                value(row, Qt::UserRole + 1) :
                value(row, m_pkeyRole);
}

bool SqlModel::removeRow(int row)
{
    return QAbstractItemModel::removeRow(row, QModelIndex());
}

void SqlModel::refresh()
{
    if (!m_query) {
        return;
    }

    setQuery(m_query->lastQuery());
}


QModelIndex SqlModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex SqlModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int SqlModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_query ? m_query->size() : 0;
}

int SqlModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_query ? m_query->record().count() : 0;
}

QVariant SqlModel::data(const QModelIndex &index, int role) const
{
    if (!m_query || !index.isValid()) {
        QVariant();
    }

    return value(index.row(), role);
}

QHash<int, QByteArray> SqlModel::roleNames() const
{
    return m_roles;
}

bool SqlModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count)
    Q_UNUSED(parent)

    if (!m_deletedIds.isEmpty()) {
        m_deletedIds.append(",");
    }
    m_deletedIds.append(primaryKeyValue(row).toString() );

    return true;
}

bool SqlModel::submit()
{
    if (m_tableName.isEmpty() || m_deletedIds.isEmpty()) {
        return true;
    }

    QSqlQuery sql;
    sql.exec(QString("DELETE FROM %1 WHERE %2 IN (%3)")
             .arg(m_tableName)
             .arg(m_pkeyRole.isEmpty() ? m_query->record().field(0).name() : m_pkeyRole)
             .arg(m_deletedIds));

    m_deletedIds.clear();

    if (sql.lastError().isValid()) {
        m_errorString = sql.lastError().text();
        return false;
    }

    refresh();
    return true;
}

void SqlModel::sort(int column, Qt::SortOrder order)
{
    m_query->first();
    qDebug() << column << m_query->record().field(column).name();
    QString orderBy = " ORDER BY ";
    orderBy += m_query->record().field(column).name();
    orderBy += order == Qt::AscendingOrder ? " ASC" : " DESC";
    setOrderBy(orderBy);
}
