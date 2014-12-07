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

#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSet>
#include <QtSql>
#include <QHashIterator>

#include "core_global.h"

namespace Core {

class CORE_EXPORT User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rolename READ rolename WRITE setRolename NOTIFY rolenameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int contractor READ contractorId WRITE setContractor NOTIFY contractorChanged)
public:
    explicit User(QObject *parent = 0);
    explicit User(QString rolename, QObject *parent = 0);

    QString rolename() const { return m_rolename; }
    void setRolename(const QString &role);

    QString name() const { return m_name; }
    void setName(const QString &value);

    int contractorId() const { return m_contractorId; }
    void setContractor(int id);

    int location() const;

    bool changePassword(const QString &pwd);
    bool changePassword(const QString role, const QString &pwd);


    QJsonObject parameters() const;
    QJsonObject parameter(const QString &name) const;
    QJsonValue parameterValue(const QString &name) const;
    void setParameter(const QString &name, QVariant &value);
    void setParameter(const QString &name, QVariantMap &value);
    void setParameters(const QString &params);

    QString gui() const;
    void setGui(const QString &gui);

    QString errorString() const { return m_errorString; }


signals:
    void rolenameChanged();
    void nameChanged();
    void contractorChanged();
    
public slots:
    bool load();
    bool save();

protected:
    QString m_rolename;
    QString m_name;
    int m_contractorId;
    QVariantMap m_paramsMap;
    QJsonDocument m_jsonDoc;
    int m_location;
    QString m_gui;
    QString m_errorString;
    bool m_isExists;
    
};

}

#endif // USER_H
