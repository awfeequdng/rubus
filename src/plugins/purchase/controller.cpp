#include "controller.h"
#include "core.h"
#include "field.h"

#include <QDebug>

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}

QVariant Controller::objectId() const
{
    return m_id;
}

void Controller::setSchemeObject(const QString &objectName)
{
    m_schemeObject = objectName;
    parseScheme(m_schemeObject);
}

QString Controller::schemeObject() const
{
    return m_schemeObject;
}

void Controller::setUi(QObject *ui)
{
    m_ui = ui;
}

QObject *Controller::ui() const
{
    return m_ui;
}

void Controller::bindData(QObject *source, const QString &propertyName, const QString &field)
{
    SourceData data;
    data.object = source;
    data.property = propertyName;
    m_binding.insert(field, data);
}

QString Controller::errorString() const
{
    return m_errorString;
}

bool Controller::load(const QVariant &id)
{
    setData("qty", 56);
    return true;
}

bool Controller::save(const QVariant &id)
{
    qDebug() << data("qty");
    return false;
}

void Controller::parseScheme(const QString &object)
{
    if (object.isEmpty()) {
        return;
    }

    m_fields = Core::ICore::fields(object);
}

QVariant Controller::data(const QString &field)
{
    return m_binding.value(field).object->property(m_binding.value(field).property.toLatin1());
}

void Controller::setData(const QString &field, const QVariant &value)
{
    QObject *obj = m_binding.value(field).object;
    if (!obj) {
        return;
    }

    if (!obj->setProperty(m_binding.value(field).property.toLatin1(), value)) {
        qWarning() << "Property '" << m_binding.value(field).property << "'is not defined!";
    }
}

