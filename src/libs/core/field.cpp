#include "field.h"
#include "core.h"

#include <QJsonObject>
#include <QDebug>

Field::Field(QObject *parent) :
    QObject(parent)
{
}

Field::Field(const QString &fieldName, const QString &schemeObject, const QJsonObject &scheme, QObject *parent) :
    QObject(parent)
{
    load (fieldName, schemeObject, scheme);
}

void Field::load(const QString &fieldName, const QString &schemeObject, const QJsonObject &scheme)
{
    QJsonObject _scheme = scheme.isEmpty() ? Core::ICore::scheme() : scheme;

    QJsonObject f = _scheme.value(schemeObject).toObject()
            .value("fields").toObject()
            .value(fieldName).toObject();
    if (f.isEmpty()) {
        qWarning() << "In object \"" << schemeObject << "\" field \"" << fieldName << "\" not found!";
        return;
    }

    m_name = fieldName;
    m_title = f.value("title").toString();
    m_lookup = f.contains("lookup");
    m_type = m_lookup ?
                QVariant::String : //TODO fix
                typeFromString(f.value("type").toString());
    m_tableFieldName =f.value("dbname").toString();
    m_format = f.value("format").toString();
}

QString Field::name() const
{
    return m_name;
}

void Field::setName(const QString &name)
{
    m_name = name;
}

QString Field::tableFieldName()
{
    return m_tableFieldName;
}

void Field::setTableFieldName(const QString &name)
{
    m_tableFieldName = name;
}

QVariant::Type Field::type() const
{
   return m_type;
}

void Field::setType(QVariant::Type type)
{
    m_type = type;
}

QString Field::title() const
{
    return m_title;
}

void Field::setTitle(const QString &title)
{
    m_title = title;
}

QString Field::format() const
{
    return m_format;
}

void Field::setFormat(const QString &format)
{
    m_format = format;
}

QVariant::Type Field::typeFromString(const QString &type)
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
