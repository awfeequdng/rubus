#include "advschemetableview.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

AdvSchemeTableView::AdvSchemeTableView(QWidget *parent) :
    AdvTableView(parent)
{
}

void AdvSchemeTableView::setScheme(const QString &json)
{
    m_scheme = json;
    parseScheme(m_scheme, m_schemeObject);
}

QString AdvSchemeTableView::scheme() const
{
    return m_scheme;
}

void AdvSchemeTableView::setSchemeObject(const QString &objectName)
{
    m_schemeObject = objectName;
    parseScheme(m_scheme, m_schemeObject);
}

QString AdvSchemeTableView::schemeObject() const
{
    return m_schemeObject;
}

void AdvSchemeTableView::parseScheme(const QString &json, const QString &objectName)
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

    obj = obj.value("tableview").toObject();
    if (obj.isEmpty()) {
        return;
    }

    obj = obj.value("columns").toObject();
    if (obj.isEmpty()) {
        return;
    }

    QJsonArray aOrder = obj.value("order").toArray();
    QJsonArray aHidden = obj.value("hidden").toArray();
    QJsonObject objWidth = obj.value("width").toObject();
    if (aOrder.isEmpty()) {
        return;
    }

    m_columns.clear();

    for(int i = 0; i < aOrder.count(); i++) {
        m_columns.append(aOrder.at(i).toString());
        bool hidden = aHidden.contains(aOrder.at(i));
        horizontalHeader()->setSectionHidden(i, hidden);

        int w = objWidth.value(aOrder.at(i).toString()).toInt(-1);
        horizontalHeader()->setDefaultSectionSize(i, w);
    }
}
