/***************************************************************************
 *   This file is part of the Rubus project                                *
 *   Copyright (C) 2012-2014 by Ivan Volkov                                *
 *   wulff007@gmail.com                                                    *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "user.h"
#include "qjsonarray.h"
#include "qjsonobject.h"


using namespace Core;

User::User(QString rolename, QObject *parent) :
    QObject(parent),
    m_rolename(rolename),
    m_contractorId(-1)
{
}

bool User::load()
{
    QSqlQuery sql;

    //CONTACTS
    sql.exec(QString("SELECT up_name,up_contractor,up_params, up_gui "
                     "FROM user_params WHERE up_role = '%1'")
             .arg(rolename()));

    if (sql.lastError().isValid()) {
        qDebug() << sql.lastError();
        return false;
    }

    if (sql.next()) {
        setName(sql.value(0).toString());
        m_contractorId = sql.value(1).isNull() ? -1 : sql.value(1).toInt();

        QJsonParseError err;
        m_jsonDoc = QJsonDocument::fromJson(sql.value(2).toString().toUtf8(), &err);

        if (err.error != QJsonParseError::NoError) {
            qDebug() << "can't parce parameter's user: JsonError:" << err.error;
        }

        m_gui = sql.value(3).toString();
        m_location = parameterValue("location").toVariant().toInt();
    } else {
        qWarning() << "No record of user contacts";
    }

    return true;
}

int Core::User::location() const
{
    return m_location;
}

QJsonObject User::parameters() const
{
    return m_jsonDoc.object();
}

QJsonObject User::parameter(const QString &name) const
{
    QJsonObject obj = m_jsonDoc.object();
    return obj[name].toObject();
}

QJsonValue Core::User::parameterValue(const QString &name) const
{
    QJsonObject obj = m_jsonDoc.object();
    return obj[name];
}

void User::setParameter(const QString &name, QVariant &value)
{
    m_jsonDoc.object()[name] = QJsonValue::fromVariant(value);
}

void Core::User::setParameter(const QString &name, QVariantMap &value)
{
    m_jsonDoc.object()[name] = QJsonObject::fromVariantMap(value);

}

QString User::gui() const
{
    return m_gui;
}

void User::setGui(const QString &gui)
{
    m_gui = gui;
}


bool User::save()
{
    QSqlQuery sql;

    if (!sql.exec(QString("UPDATE user_params SET up_params = '%1' WHERE up_role = '%2'")
                  .arg(QString(m_jsonDoc.toJson()))
                  .arg(rolename()))) {
        qCritical() << sql.lastError();
        m_errorString = sql.lastError().text();
        return false;
    }

    return true;
}
