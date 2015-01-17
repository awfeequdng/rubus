#ifndef REQUESTMODEL_H
#define REQUESTMODEL_H

#include <QAbstractItemModel>

struct ModelField {
    QString name;
    QString dbName;
    QVariant::Type type;
    QString title;
    QString format;
    bool lookup;
};

class QSqlQuery;

class RequestModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RequestModel(QObject *parent = 0);

    void setScheme(const QString &json);
    QString scheme() const;

    void setSchemeObject(const QString &objectName);
    QString schemeObject() const;

    bool populate();
    QString errorString() const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);

signals:

public slots:

private:
    QString m_scheme;
    QString m_schemeObject;

    QString m_selectQuery;
    QMap<QString, ModelField*> m_fieldByName;
    QStringList m_columns;
    QMap<int, Qt::Alignment> m_columnAlign;
    QSqlQuery *m_sql;
    QString m_errorString;


private:
    void parseScheme(const QString &json, const QString &objectName);
    QString generateSelectQuery() const;
    void parseFields(QJsonObject object);
    void parseColumns(QJsonObject object);
    QVariant::Type typeFromString(const QString &type);
};

#endif // REQUESTMODEL_H
