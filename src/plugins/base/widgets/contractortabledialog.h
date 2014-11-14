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
#ifndef CONTRACTORTABLEDIALOG_H
#define CONTRACTORTABLEDIALOG_H

#include "widgets/tabledialog.h"
#include "report.h"

namespace Ui {
class ContractorTableDialog;
}

class ContractorModel;
class ContractorEditWidget;

class ContractorTableDialog : public TableDialog
{
    Q_OBJECT

public:
    explicit ContractorTableDialog(QWidget *parent = 0);
    ~ContractorTableDialog();

public slots:
    void slotPrint(Report &r);

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::ContractorTableDialog *ui;
    ContractorModel *m_model;
    ContractorEditWidget *m_editWdg;
};

#endif // CONTRACTORTABLEDIALOG_H
