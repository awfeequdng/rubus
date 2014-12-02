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
#include "oooreportbuilder.h"

#include <QMessageBox>
#include <QApplication>

OOoReportBuilder::OOoReportBuilder(QObject *parent) :
    QObject(parent),
    m_isLoaded(false)
{
}

void OOoReportBuilder::showReport()
{
    QDesktopServices::openUrl(
                QUrl("file:///"+m_readyReportFile));
}

void OOoReportBuilder::parse(bool show)
{
    if (!m_isLoaded) {
        return;
    }

    parseDocument();
    saveContent();
    saveReport();

    if (show)
        showReport();
}


void OOoReportBuilder::loadContentFile(QString file)
{
    m_isLoaded = false;

    if (file.isEmpty())
        return;

    if (!QFile::exists(file)) {
        QMessageBox::critical(QApplication::activeWindow(),"loadContentFile",
                              tr("File '%1' not found").arg(file));
        return;
    }

    m_fileContent = file;
    QFile f(file);

    if(f.open(QIODevice::ReadOnly)) {
        m_isLoaded = m_doc.setContent(&f);


    }
    f.close();
}

void OOoReportBuilder::saveContent()
{
    QFile f(m_fileContent);
    if(f.open(QIODevice::WriteOnly)) {

        QTextStream out(&f);
        out.setCodec(QTextCodec::codecForName("utf-8"));
        out  << m_doc.toString(4);
        f.close();
    }
}

void OOoReportBuilder::saveReport()
{
    m_readyReportFile.clear();

    QFile file(m_filename);
    QFileInfo info(file);
    QString newFile = m_outputDir + "\\" + info.baseName() + QString("_gen%1.%2")
            .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"))
            .arg(info.completeSuffix());
    if (!file.copy(newFile)) {
        QMessageBox::critical(QApplication::activeWindow(),
                              "saveReport",tr("Can't copy file! "));
        return;
    }

    m_readyReportFile = newFile;

    QStringList args;
    args << "a" << newFile
         << m_outputDir + "\\content.xml";

    QProcess *arh = new QProcess();

    arh->execute("7z", args);

    QFile f(m_outputDir + "\\content.xml");
    f.remove();
}



void OOoReportBuilder::finishedArh ( int exitCode, QProcess::ExitStatus /*exitStatus*/ )
{
    if (exitCode > 0) {
        QMessageBox::critical(QApplication::activeWindow(),
                              "loadReportFile",
                              tr("Error unpack '%1' code:%1 ")
                              .arg(m_filename)
                              .arg(exitCode));
    } else
        loadContentFile(m_fileContent);
}

void OOoReportBuilder::loadReportFile(QString filename)
{
    m_isLoaded = false;
    m_filename = filename;

    if (m_filename.isEmpty())
        return;

    if (!QFile::exists(m_filename)) {
        QMessageBox::critical(QApplication::activeWindow(),"loadReportFile",
                              tr("File '%1' not found! ").arg(m_filename));
        return;
    }

    m_fileContent = m_outputDir + "\\content.xml";

    QStringList args;
    args << "e" << m_filename << QString("-o%1")
            .arg(m_outputDir)
         << "content.xml" << "-y";

    QProcess *arh = new QProcess();

    int exitCode = arh->execute("7z", args);


    if (exitCode == 0) {
        m_isLoaded = true;
        loadContentFile(m_fileContent);
    } else {
        if (exitCode == -2) {
            QMessageBox::critical(QApplication::activeWindow(),"loadReportFile",
                                  tr("Can'c create process unpacking! "));
        } else if (exitCode == -1) {
                QMessageBox::critical(QApplication::activeWindow(),"loadReportFile",
                                      tr("Error in unpacking! "));
        } else if (exitCode == 1) {
                QMessageBox::critical(QApplication::activeWindow(),"loadReportFile",
                                      tr("Error in unpacking (ExitCode: 1 - Warning)"));
        } else if (exitCode == 2) {
                QMessageBox::critical(QApplication::activeWindow(),"loadReportFile",
                                      tr("Error in unpacking (ExitCode: 2 - Fatal Error)"));
        } else {
                QMessageBox::critical(QApplication::activeWindow(),"loadReportFile",
                                      tr("Unknown error (ExitCode: %1)")
                                      .arg(exitCode));
        }
    }
}

QString OOoReportBuilder::cellText(const QDomElement cell)
{
    return cell.firstChild().toElement().text();
}

void OOoReportBuilder::setText(const QDomElement &element, const QString text)
{
    if (element.isNull())
        return;

    QDomElement el = element;

    el.removeChild(el.firstChild());
    QDomElement newElement = m_doc.createElement("text:p");
    QDomText newTitleText = m_doc.createTextNode(text);
    newElement.appendChild(newTitleText);
    el.appendChild(newElement);


}

void OOoReportBuilder::setText(const QDomElement &element, const double value)
{
    if (element.isNull())
        return;

    QDomElement el = element;

    el.setAttribute("office:value-type", "float");
    el.setAttribute("office:value", value);

    el.removeChild(el.firstChild());
    QDomElement newElement = m_doc.createElement("text:p");

    QDomText newTitleText = m_doc.createTextNode(QLocale::system().toString(value, 'f', 2));
    newElement.appendChild(newTitleText);
    el.appendChild(newElement);
}

void OOoReportBuilder::parseDocument()
{
    if (m_doc.isNull())
        return;

    QDomNodeList tables = m_doc.documentElement()
            .elementsByTagName("table:table");

    for (int i = 0; i < tables.count(); i++) {
        parseTable(tables.item(i));
    }
}

void OOoReportBuilder::parseTable(QDomNode table)
{
    QDomNode child = table.firstChild();

    while (!child.isNull()) {
        if (child.nodeName() == "table:table-row") {
            child = parseRow(child);
        }

        child = child.nextSibling();
    }
}

QDomElement OOoReportBuilder::parseRow(QDomNode row)
{
    QDomElement lastElement = row.toElement();

    if (reportBand(row) == Detail) {
        lastElement = processDetail(row.toElement());
    } else {
        QDomNode child = row.firstChild();

        while (!child.isNull()) {
            if (child.nodeName() == "table:table-cell") {
                QString str = processString(cellText(child.toElement()));
                bool ok;
                double value = str.toDouble(&ok);

                if (ok) {
                   setText(child.toElement(), value);
                } else
                    setText(child.toElement(), str);
            }

            child = child.nextSibling();
        }
    }

    return lastElement;
}

ReportBands OOoReportBuilder::reportBand(QDomNode row)
{
    if (row.nodeName() != "table:table-row")
        return None;

    QString text = cellText(row.firstChild().toElement());
    QRegExp rx("\\{\\w+\\}");

    int pos = rx.indexIn(text);

    if (pos != -1) {
        text = text.mid(pos + 1,rx.matchedLength() - 2);

        if (!QString::compare(text,"rep_header",Qt::CaseInsensitive))
            return ReportHeader;
        else if (!QString::compare(text,"rep_footer",Qt::CaseInsensitive))
            return ReportFooter;
        else {

            text = text.remove(QRegExp("\\d+"));

            if (!QString::compare(text,"header",Qt::CaseInsensitive))
                return DetailHeader;
            else if (!QString::compare(text,"footer",Qt::CaseInsensitive))
                return DetailFooter;

            else if (!QString::compare(text,"detail",Qt::CaseInsensitive))
                return Detail;
        }
    }

    return None;
}

QString OOoReportBuilder::processString(const QString source)
{
    if (source.isEmpty())
        return QString();

    QString text = source;
    QRegExp rx("\\{\\w+:?\\w+\\}");
    int pos = 0;



    while ((pos = rx.indexIn(text,pos)) != -1) {

        QString replaced = processWord(text.mid(pos,rx.matchedLength()));
        text.replace(pos,rx.matchedLength(),replaced);

        pos += qMin(rx.matchedLength(),replaced.length());
    }

    //qDebug() << text;
    return text;
}

QString OOoReportBuilder::processWord(const QString source)
{
    if (source.isEmpty())
        return QString();

    QString text = source;
    text.remove(QRegExp("\\{|\\}"));

    if (text[0] == 'P') {
        return processParams(text.mid(2));
    }

    return source;

}

QString OOoReportBuilder::processParams(QString paramName)
{
    return m_params.value(paramName,"Not found").toString();
}

QDomElement OOoReportBuilder::processDetail(const QDomElement &rowDetail)
{
    QDomElement lastElement = rowDetail;

    if (rowDetail.isNull() || reportBand(rowDetail) != Detail)
        return QDomElement();

    QString textCell = cellText(rowDetail.firstChild().toElement());
    textCell.remove(QRegExp("\\{|\\}|detail"));
    int modelId = textCell.toInt() - 1;

    if (modelId < m_models.count() - 1) {
        rowDetail.parentNode().removeChild(rowDetail);
        return lastElement.previousSibling().toElement();
    }

    QAbstractItemModel *model = m_models.at(modelId);

    for (int i = 0; i < model->rowCount(); i++) {
        QDomElement tmpRow = rowDetail.cloneNode(true).toElement();

        QDomElement cell = tmpRow.firstChild().toElement();
        cell = cell.nextSibling().toElement();

        while (!cell.isNull()) {
            QString str = cellText(cell);

            if (!str.isEmpty()) {
                str.remove(QRegExp("\\{|\\}"));

                if (!QString::compare(str,"rowno",Qt::CaseInsensitive)) {
                    setText(cell,QString::number(i));
                } else if (str[0] == 'P') {

                    QVariant var = processParams(str.mid(2));
                    if (var.type() == QVariant::Double) {
                        setText(cell,var.toDouble());
                    } else {
                        setText(cell,var.toString());
                    }
                } else {
                    QRegExp rx("col\\d{1,2}");

                    if (rx.indexIn(str) == 0) {
                        int colNo = str.remove(QRegExp("col")).toInt();
                        QVariant var = model->data(model->index(i,colNo));
                        if (colNo <= model->columnCount() - 1) {
                            if (var.type() == QVariant::Double) {
                                setText(cell, var.toDouble());
                            } else
                                setText(cell, var.toString());
                        } else
                            setText(cell,"Err");
                    } else
                        setText(cell,str);
                }
            }

            if (cell.attributes().contains("table:formula")) {
                QString formula = processFormula(cell.attribute("table:formula"), i);
                cell.setAttribute("table:formula",formula);
            }

            cell = cell.nextSibling().toElement();
        }

        lastElement = rowDetail.parentNode().insertBefore(tmpRow,rowDetail).toElement();
    }

    rowDetail.parentNode().removeChild(rowDetail);
    return lastElement;
}

void OOoReportBuilder::appendModel(QAbstractItemModel* model)
{
    m_models.append(model);
}

void OOoReportBuilder::appendParam(const QString name,const QVariant value)
{
    m_params.insert(name,value);
}

QString OOoReportBuilder::processFormula(const QString formula, int offset)
{
    //example "of:=[.C11]*[.D11]"
    QRegExp rx("\\[\\.[A-Z]{1,2}\\d+\\]");
    QString result = formula;

    int pos = 0;

    while ((pos = rx.indexIn(result,pos)) != -1) {

        QRegExp rx2("\\d+");
        int posDig = rx2.indexIn(result,pos);
        if (posDig != -1) {
            QString rowNo = QString::number(result.mid(posDig,rx2.matchedLength()).toInt() + offset);
            result.replace(posDig,rx2.matchedLength(),rowNo);
        }

        pos += rx2.matchedLength();
    }

    return result;
}






