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

    m_query->exec(query);

    if (m_query->lastError().isValid()) {
        qCritical() << m_query->lastError();
        emit errorStringChanged();
        return;
    }

    QSqlRecord rec = m_query->record();
    m_roles.clear();
    for (int i = 0; i < rec.count(); i++) {
        m_roles.insert(Qt::UserRole + i + 1, rec.fieldName(i).toUtf8());
    }

    emit queryChanged();
}

QString SqlModel::errorString() const
{
    return m_query ? m_query->lastError().text() : QString();
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

    if (!m_query->seek(index.row())) {
        qDebug() << "can't seek in data()";
        return QVariant();
    }

    QSqlRecord rec = m_query->record();
    QVariant value;

    if (role < Qt::UserRole) {
        value = rec.value(index.column());
    } else {
        value = rec.value(role - Qt::UserRole - 1);
    }

    return value;
}

QHash<int, QByteArray> SqlModel::roleNames() const
{
    return m_roles;
}
