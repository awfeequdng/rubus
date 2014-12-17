#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "core_global.h"

class QSqlQuery;

class CORE_EXPORT SqlModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString select READ select WRITE setSelect NOTIFY selectChanged)
    Q_PROPERTY(QString where READ where WRITE setWhere NOTIFY whereChanged)
    Q_PROPERTY(QString orderBy READ orderBy WRITE setOrderBy NOTIFY orderByChanged)
    Q_PROPERTY(QString primaryKeyRole READ primaryKeyRole WRITE setPrimaryKeyRole NOTIFY primaryKeyColumnChanged)
    Q_PROPERTY(QString tableName READ tableName WRITE setTableName NOTIFY tableNameChanged)
public:
    explicit SqlModel(QObject *parent = 0);

    QString query() const;
    void setQuery(const QString &query);

    QString select() const;
    void setSelect(const QString &select);

    QString where() const;
    void setWhere(const QString &where);

    QString orderBy() const;
    void setOrderBy(const QString &order);

    QString primaryKeyRole() const;
    void setPrimaryKeyRole(const QString &column);

    QString tableName() const;
    void setTableName(const QString &name);

    QString errorString() const;
    Q_INVOKABLE QVariant value(int row, int role) const;
    Q_INVOKABLE QVariant value(int row, QString rolename) const;
    Q_INVOKABLE QVariant primaryKeyValue(int row) const;
    Q_INVOKABLE bool removeRow(int row);

    Q_INVOKABLE void refresh();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool submit();
    Q_INVOKABLE void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

signals:
    void queryChanged();
    void errorStringChanged();
    void selectChanged();
    void whereChanged();
    void orderByChanged();
    void primaryKeyColumnChanged();
    void tableNameChanged();

public slots:

private:
    QSqlQuery *m_query;
    QHash<int, QByteArray> m_roles;
    QString m_select;
    QString m_where;
    QString m_order;
    QString m_pkeyRole;
    QString m_tableName;
    QString m_errorString;

    QString m_deletedIds;

};

#endif // SQLMODEL_H
