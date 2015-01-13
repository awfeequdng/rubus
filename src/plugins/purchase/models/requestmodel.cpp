#include "requestmodel.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

RequestModel::RequestModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

void RequestModel::setScheme(const QString &json)
{
    m_scheme = json;
    parseScheme(m_scheme, m_schemeObject);
}

QString RequestModel::scheme() const
{
   return m_scheme;
}

void RequestModel::setSchemeObject(const QString &objectName)
{
    m_schemeObject = objectName;
    parseScheme(m_scheme, m_schemeObject);
}

QString RequestModel::schemeObject() const
{
    return m_schemeObject;
}

QModelIndex RequestModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex RequestModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int RequestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 0;
}

int RequestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columns.count();
}

QVariant RequestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QVariant();
    }

    return QVariant();
}

bool RequestModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

QVariant RequestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_columns.isEmpty()
            && orientation == Qt::Horizontal
            && role == Qt::DisplayRole) {
        qDebug() << m_columns.at(section);
        //return m_fieldByName.value(m_columns.at(section))->title;
        return QString();
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

bool RequestModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    Q_UNUSED(row)
    Q_UNUSED(count)
    return false;
}

void RequestModel::parseScheme(const QString &json, const QString &objectName)
{
    if (json.isEmpty() || objectName.isEmpty()) {
        return;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);

    if (err.error) {
        qCritical() << "line:" << err.offset << "err: " << err.error << ":" << err.errorString();
        return;
    }

   if (doc.isEmpty()) {
       qCritical() << tr("Scheme is empty!");
       return;
   }

   if (!doc.object().contains(objectName)) {
       qCritical() << tr("No object \"%1\" in scheme!").arg(objectName);
       return;
   }

   QJsonObject obj = doc.object().value(objectName).toObject();

   if (obj.isEmpty()) {
       qCritical() << tr("Object \"%1\" is empty!").arg(objectName);
       return;
   }

   parseFields(obj);
   parseColumns(obj);

   QJsonObject sqlObj = obj.value("sql").toObject();

   m_selectQuery = sqlObj.contains("select")
           ? sqlObj.value("select").toString()
           : generateSelectQuery();
}

QString RequestModel::generateSelectQuery() const
{
    return QString();
}

void RequestModel::parseFields(QJsonObject object)
{
    qDeleteAll(m_fieldByName);
    m_fieldByName.clear();

    QJsonObject fields = object.value("fields").toObject();
    if (fields.isEmpty()) {
        return;
    }

    QMapIterator<QString, QVariant> i(fields.toVariantMap());
    while (i.hasNext()) {
        i.next();
        QJsonObject obj = i.value().toJsonObject();
        ModelField *field = new ModelField;
        field->name = i.key();
        field->title = obj.value("title").toString();
        field->type = QVariant::String;
        field->dbName = obj.value("dbname").toString();

        m_fieldByName.insert(field->name, field);
    }
}

void RequestModel::parseColumns(QJsonObject object)
{
    m_columns.clear();

    QJsonObject colsObj = object.value("tableview").toObject();
    if (colsObj.isEmpty()) {
        return;
    }
    colsObj = colsObj.value("columns").toObject();
    if (colsObj.isEmpty()) {
        return;
    }

    QJsonArray arr = colsObj.value("order").toArray();
    if (arr.isEmpty()) {
        return;
    }

    for(int i = 0; i < arr.count(); i++) {
        m_columns.append(arr.at(i).toString());
    }
}
