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
public:
    explicit SqlModel(QObject *parent = 0);

    QString query() const;
    void setQuery(const QString &query);

    QString errorString() const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

signals:
    void queryChanged();
    void errorStringChanged();

public slots:

private:
    QSqlQuery *m_query;
    QHash<int, QByteArray> m_roles;

};

#endif // SQLMODEL_H
