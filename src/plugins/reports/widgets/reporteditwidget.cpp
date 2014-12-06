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
#include "reporteditwidget.h"
#include "ui_reporteditwidget.h"
#include "models/reporttypemodel.h"
#include "report.h"

#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>

ReportEditWidget::ReportEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportEditWidget)
{
    ui->setupUi(this);

    m_menuModel = new QSqlQueryModel(this);
    m_typeModel = new ReportTypeModel(this);

    ui->cmbType->setModel(m_typeModel, 0, 1);

    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(closed()));
    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnChooseSourceFile, SIGNAL(clicked()), SLOT(chooseFile()));
    connect(ui->btnSaveToFile, SIGNAL(clicked()), SLOT(saveToFile()));
}

ReportEditWidget::~ReportEditWidget()
{
    delete ui;
}

bool ReportEditWidget::load(int id)
{
    populate();
    m_id = id;
    ui->edInFile->clear();
    m_data.clear();

    if (id != -1) {
        QSqlQuery sql;
        sql.exec(QString("SELECT re_name,re_type,re_menu, re_data FROM reports WHERE re_id = %1")
                 .arg(id));

        if (sql.lastError().isValid()) {
            QMessageBox::critical(this, "Error", sql.lastError().text());
            return false;
        }

        if (!sql.next()) {
            QMessageBox::critical(this, "Error", tr("Item #%1 not found").arg(id));
            return false;
        }


        ui->edId->setValue(id);
        ui->edName->setText(sql.value(0).toString());
        ui->cmbType->setCurrentKey(sql.value(1).toInt());
        ui->cmbMenu->lineEdit()->setText(sql.value(2).toString());
        m_data = sql.value(3).toString();
    } else {
        ui->edId->setValue(0);
        ui->edName->setText(QString());
        ui->cmbType->setCurrentKey(Report::NcReportEngine);
        ui->cmbMenu->setCurrentIndex(-1);
    }

    return true;
}

bool ReportEditWidget::save()
{
    if (!isValid()) {
        return false;
    }

    if(!ui->edInFile->text().isEmpty()) {
        QFile f(ui->edInFile->text());
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Open file"), f.errorString());
            return false;
        }

        m_data = f.readAll();
        f.close();
    }

    int new_id = ui->edId->value();

    QSqlQuery sql;

    if (new_id <= 0) {
        sql.exec("SELECT nextval(pg_get_serial_sequence('reports', 're_id'))");
        sql.next();

        if (sql.lastError().isValid()) {
            qCritical() << sql.lastError();
        }

        new_id = sql.value(0).toInt();
    }



    if (m_id == -1) {
        sql.prepare("INSERT INTO reports (re_id,re_name,re_type,re_menu,re_data) "
                    "VALUES (:id,:name,:type,:menu,:data)");
    } else {
        sql.prepare("UPDATE reports SET re_id = :id,re_name = :name,re_type = :type,re_menu = :menu,re_data = :data "
                    "WHERE re_id = :old_id");
        sql.bindValue(":old_id",m_id);
    }




    sql.bindValue(":id", new_id);
    sql.bindValue(":name",ui->edName->text());
    sql.bindValue(":type",ui->cmbType->currentKey().toInt());
    sql.bindValue(":menu",ui->cmbMenu->currentText());
    sql.bindValue(":data", m_data);

    if (!sql.exec()) {
        QMessageBox::critical(this, tr("Error"), sql.lastError().text());
        return false;
    }

    m_id = new_id;


    emit saved();
    return true;
}

void ReportEditWidget::chooseFile()
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setViewMode(QFileDialog::List);

    QStringList filters;
    filters << tr("All reports (*.xml *.ncr *.ods)");
    filters << tr("NcReport (*.ncr)");
    filters << tr("CuteReport (*.xml)");
    filters << tr("OO Calc (*.ods)");
    filters << tr("All files (*.*)");
    dlg.setNameFilters(filters);

    dlg.selectFile(ui->edInFile->text());
    if (dlg.exec() == QDialog::Accepted) {
        ui->edInFile->setText(QDir::toNativeSeparators(dlg.selectedFiles().first()));
    }
}

void ReportEditWidget::saveToFile()
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setViewMode(QFileDialog::List);

    QStringList filters;
    filters << tr("All reports (*.xml *.ncr *.ods)");
    filters << tr("NcReport (*.ncr)");
    filters << tr("CuteReport (*.xml)");
    filters << tr("OO Calc (*.ods)");
    filters << tr("All files (*.*)");
    dlg.setNameFilters(filters);

    QString ext;
    QString nameFilter;
    int type = ui->cmbType->currentKey().toInt();
    switch (type) {
    case Report::NcReportEngine :
        ext = ".ncr";
        nameFilter = filters.at(1);
        break;
    case Report::CuteReportEngine :
        ext = ".xml";
        nameFilter = filters.at(2);
        break;
    case Report::OpenOfficeEngine :
        ext = ".ods";
        nameFilter = filters.at(3);
        break;
    default:
        break;
    }

#if QT_VERSION < 0x050000
    dlg.selectFilter(nameFilter);
#else
    dlg.setNameFilter(nameFilter);
#endif

    dlg.selectFile(ui->edName->text() + ext);
    if (dlg.exec() == QDialog::Accepted) {
        QFile f(dlg.selectedFiles().first());
        if (!f.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Save"), f.errorString());
            return;
        }

        f.write(m_data.toUtf8());
        f.close();
    }
}

void ReportEditWidget::populate()
{
    m_menuModel->setQuery(QString("SELECT re_menu FROM reports GROUP BY re_menu"));

    if (m_menuModel->lastError().isValid()) {
        qDebug() << m_menuModel->lastError();
    }
    ui->cmbMenu->setModel(m_menuModel, 0, 0);
}

bool ReportEditWidget::isValid()
{
    if (ui->edName->text().simplified().isEmpty()) {
        QMessageBox::critical(this, tr("Valid"), tr("Name is empty!"));
        return false;
    }

    if (!ui->edInFile->text().isEmpty() && !QFile::exists(ui->edInFile->text())) {
        QMessageBox::critical(this, tr("Valid"), tr("File not found!"));
        return false;
    }

    return true;
}
