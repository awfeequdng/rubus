#include "requestmodel.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QtSql>

RequestModel::RequestModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_sql(0)
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

bool RequestModel::populate()
{
    if (m_selectQuery.isEmpty()) {
        m_errorString = tr("Select query is empty!");
        return false;
    }

    if (!m_sql) {
        m_sql = new QSqlQuery();
    }

    emit beginResetModel();
    m_sql->exec(m_selectQuery);
    emit endResetModel();

    if (m_sql->lastError().isValid()) {
        m_errorString = m_sql->lastError().text();
        return false;
    }

    return true;
}

QString RequestModel::errorString() const
{
    return m_errorString;
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
    return m_sql ? m_sql->size() : 0;
}

int RequestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columns.count();
}

QVariant RequestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_sql) {
        return QVariant();
    }

    if (!m_sql->seek(index.row())) {
        qWarning() << QString("can't do seek");
        return QVariant();
    }

    QSqlRecord rec = m_sql->record();
    ModelField *f = m_fieldByName.value(m_columns.at(index.column()));

    if (!f) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QSqlField sqlf = rec.field(f->dbName);
        return sqlf.isValid() ? sqlf.value() : tr("No Field");
    }

    if (role == Qt::TextAlignmentRole) {
        return headerData(index.column(), Qt::Horizontal, role);
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
    if (!m_columns.isEmpty() && orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            ModelField *f = m_fieldByName.value(m_columns.at(section));
            return f ? f->title : m_columns.at(section) + "?";
        }

        if (role == Qt::TextAlignmentRole) {
            return QVariant(m_columnAlign.value(section, Qt::AlignLeft | Qt::AlignVCenter));
        }
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
        QJsonObject obj = QJsonObject::fromVariantMap(i.value().toMap());
        ModelField *field = new ModelField;
        field->name = i.key();
        field->title = obj.value("title").toString();
        field->lookup = obj.contains("lookup");
        field->type = field->lookup ?
                    QVariant::String :
                    typeFromString(obj.value("type").toString());
        field->dbName = obj.value("dbname").toString();
        field->format = obj.value("format").toString();

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

        ModelField *f = m_fieldByName.value(arr.at(i).toString());
        if (f) {
            Qt::Alignment align;
            if (f->type == QVariant::Double || f->type == QVariant::Int) {
                align = Qt::AlignVCenter | Qt::AlignRight;
            } else if (f->type == QVariant::DateTime || f->type == QVariant::Date || f->type == QVariant::Time) {
                align = Qt::AlignVCenter | Qt::AlignHCenter;
            } else {
                align = Qt::AlignVCenter | Qt::AlignLeft;
            }

            m_columnAlign.insert(i, align);
        }
    }
}

QVariant::Type RequestModel::typeFromString(const QString &type)
{
    if (type.isEmpty()) {
        return QVariant::String;
    }

    QVariant::Type t = QVariant::nameToType(type.toLatin1());

    if (t != QVariant::Invalid) {
        return t;
    }

    if (type.toLower() == QString("string")) {
        t = QVariant::String;
    } else if (type.toLower() == QString("date")) {
        t = QVariant::Date;
    } else if (type.toLower() == QString("datetime")) {
        t = QVariant::DateTime;
    } else if (type.toLower() == QString("time")) {
        t = QVariant::Time;
    } else {
        t = QVariant::Invalid;
    }

    return t;
}
