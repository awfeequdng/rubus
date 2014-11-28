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
#ifndef BASE_H
#define BASE_H

#include "base_global.h"
#include "iplugin.h"

class QAction;
class StandardTableDialog;
class ItemModel;
class ContractorTableDialog;
class UserTableDialog;

class BASE_EXPORT BasePlugin : public Core::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)

#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "BasePlugin")
#endif

public:
    explicit BasePlugin(QObject *parent = 0);
    ~BasePlugin();

    static BasePlugin *instance();

    QString name() const;
    int version() const;
    bool initialize();

public slots:
    static void showUserManager();
    static void showLocationTable();
    static void showContractorTable();
    static void showItemTable();


private:
    QAction *m_acUserManager;
    QAction *m_acReportManager;
    QAction *m_acLocations;
    QAction *m_acContractors;
    QAction *m_acItems;

    StandardTableDialog *m_itemDialog;
    ItemModel *m_itemModel;
    ContractorTableDialog *m_contractorDialog;
    UserTableDialog *m_userTableDialog;
};

#endif // BASE_H
