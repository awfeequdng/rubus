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

#include <QCryptographicHash>


//using namespace Core;

User::User(QObject *parent) :
    QObject(parent),
    m_rolename(QString()),
    m_contractorId(-1),
    m_isExists(false)
{
}

User::User(QString rolename, QObject *parent) :
    QObject(parent),
    m_rolename(rolename),
    m_contractorId(-1),
    m_isExists(false)
{
}

bool User::load()
{
    QSqlQuery sql;
    sql.exec(QString("SELECT up_name,up_contractor,up_params "
                     "FROM user_params WHERE up_role = '%1'")
             .arg(rolename()));

    m_isExists = false;

    if (sql.lastError().isValid()) {
        qDebug() << sql.lastError();
        m_errorString = sql.lastError().text();
        return false;
    }

    if (sql.next()) {
        setName(sql.value(0).toString());
        m_contractorId = sql.value(1).isNull() ? -1 : sql.value(1).toInt();

        setParameters(sql.value(2).toString());

        m_location = parameterValue("location").toVariant().toInt();
        m_isExists = true;
    } else {
        //TODO init data user
    }

    return true;
}

void User::setRolename(const QString &role)
{
    m_rolename = role;
}

void User::setName(const QString &name)
{
    m_name = name;
}

void User::setContractor(int id)
{
    m_contractorId = id;
}

int User::location() const
{
    return m_location;
}

bool User::changePassword(const QString &pwd)
{
    return changePassword(m_rolename, pwd);
}

bool User::changePassword(const QString role, const QString &pwd)
{
    if (role.isEmpty() || pwd.isEmpty()) {
        m_errorString = tr("role name or password is empty!");
        return false;
    }

    QByteArray p;
    p.append(pwd);
    p.append(role);
    p = QCryptographicHash::hash(p, QCryptographicHash::Md5).toHex();

    QSqlQuery sql;
    sql.exec(QString("ALTER ROLE %1 WITH ENCRYPTED PASSWORD 'md5%2'")
             .arg(role)
             .arg(QString(p)));

    if (sql.lastError().isValid()) {
        m_errorString = sql.lastError().text();
        return false;
    }

    return true;
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

QJsonValue User::parameterValue(const QString &name) const
{
    QJsonObject obj = m_jsonDoc.object();
    return obj[name];
}

void User::setParameter(const QString &name, QVariant &value)
{
    m_jsonDoc.object()[name] = QJsonValue::fromVariant(value);
}

void User::setParameter(const QString &name, QVariantMap &value)
{
    m_jsonDoc.object()[name] = QJsonObject::fromVariantMap(value);

}

void User::setParameters(const QString &params)
{
    QJsonParseError err;
    m_jsonDoc = QJsonDocument::fromJson(params.toUtf8(), &err);

    if (err.error != QJsonParseError::NoError) {
        qDebug() << "can't parse parameter's user: JsonError:" << err.error << err.errorString();
    }
}

bool User::permission(const QString &name) const
{
    return m_permissions.contains(name);
}

bool User::save()
{
    QSqlQuery sql;

    if (!m_isExists) {
        sql.prepare("INSERT INTO user_params (up_role, up_name, up_contractor, up_params) "
                    "VALUES (:role, :name, :contractor, :params)");
    } else {
        sql.prepare("UPDATE user_params SET up_name = :name, "
                    "up_contractor = :contractor, "
                    "up_params = :params "
                    "WHERE up_role = :role");
    }

    sql.bindValue(":role", m_rolename);
    sql.bindValue(":name", m_name);
    sql.bindValue(":contractor", m_contractorId);
    sql.bindValue(":params", m_jsonDoc.isEmpty() ? QString() : QString(m_jsonDoc.toJson()));

    if (!sql.exec()) {
        qCritical() << sql.lastError();
        m_errorString = sql.lastError().text();
        return false;
    }    

    return true;
}
