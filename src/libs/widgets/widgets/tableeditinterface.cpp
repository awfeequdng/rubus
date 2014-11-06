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
#include "tableeditinterface.h"
//#include "advtableview.h"

TableEditInterface::TableEditInterface(QWidget *parent) :
    QWidget(parent)
{
    setSettingPrefix("TableEditInterface");
    QShortcut *s_refresh = new QShortcut(this);
    s_refresh->setKey(Qt::Key_F5);
    connect(s_refresh, SIGNAL(activated()), SLOT(populate()));
}

QList<int> TableEditInterface::sourceRowsFromProxy(QModelIndexList indexes, QSortFilterProxyModel *model) const
{
    QListIterator<QModelIndex> iter(indexes);
    QList<int> rows;

    while(iter.hasNext()) {
        rows << model->mapToSource(iter.next()).row();
    }

    qSort(rows);
    return rows;
}


void TableEditInterface::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    populate();

    QSettings sett;
    restoreGeometry(sett.value(settingPrefix() + "/geometry").toByteArray());
}

void TableEditInterface::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);

    QSettings sett;
    sett.setValue(settingPrefix() + "/geometry", saveGeometry());
}

