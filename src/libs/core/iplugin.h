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
#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QVariantMap>

#include "core_global.h"
#include "core.h"


class QAction;

namespace Core {

class CORE_EXPORT IPlugin : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(int version READ version CONSTANT)
public:
    explicit IPlugin(QObject *parent = 0);

    virtual QString name() const = 0;
    virtual int version() const = 0;

    virtual bool initialize() = 0;

    virtual QList<QString> dependencyPlugins() { return QList<QString>(); }

    Core::ICore *core() const { return Core::ICore::instance(); }

    virtual QMap<QString, QAction*> actions() const { return QMap<QString, QAction*>();}

    virtual QWidget * helper() const {return 0;}

signals:

public slots:

private:
    QVariantMap m_parametrs;


};

}

Q_DECLARE_INTERFACE(Core::IPlugin,
                     "Rubus.IPlugin/1.0")

#endif // PLUGININTERFACE_H
