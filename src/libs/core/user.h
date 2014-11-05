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
#include "qjsondocument.h"

namespace Core {

struct TableRecordPrefStruct {
    QString table;
    int recordId;
    QString value;
};

class CORE_EXPORT User : public QObject
{
    Q_OBJECT
public:
    explicit User(QString rolename, QObject *parent = 0);

    bool load();

    QString rolename() { return m_rolename; }
    QString name() { return m_name; }
    int contractorId() const { return m_contractorId; }
    int location() const;

    void setName(QString value) { m_name = value; }

    QJsonObject parameters() const;
    QJsonObject parameter(const QString &name) const;
    QJsonValue parameterValue(const QString &name) const;
    void setParameter(const QString &name, QVariant &value);
    void setParameter(const QString &name, QVariantMap &value);

    QString gui() const;
    void setGui(const QString &gui);

    QString errorString() const { return m_errorString; }


signals:
    
public slots:
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
    
};

}

#endif // USER_H
