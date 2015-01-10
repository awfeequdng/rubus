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

#ifndef DLGAUTHORISE_H
#define DLGAUTHORISE_H

#include <QDialog>
#include <QMessageBox>
#include <QShowEvent>
#include "cryptor.h"
#include "dlgauthsettings.h"
#include "core.h"



namespace Ui {
class DlgAuthorise;
}

class DlgAuthorise : public QDialog
{
    Q_OBJECT    
public:
    explicit DlgAuthorise(QWidget *parent = 0);
    ~DlgAuthorise();

    void setUserName(const QString &name);
    void setPassword(const QString &pwd);

    QString username();
    bool isRemember();

public slots:
    void accept();
    int exec();
    bool authorise();




private slots:
    void showAuthSettings();
    
private:
    Ui::DlgAuthorise *ui;
    Core::ICore *m_core;

    bool m_canChangeSettingDatabase;

    void loadSettings();
    void saveSettings();


protected:
    void showEvent ( QShowEvent * event );
};

#endif // DLGAUTHORISE_H
