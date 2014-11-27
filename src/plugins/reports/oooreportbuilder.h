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
#ifndef OOOREPORTBUILDER_H
#define OOOREPORTBUILDER_H

#include <QObject>
#include <QtXml>
#include <QProcess>
#include <QtGui>
#include <QAbstractItemModel>

enum ReportBands {
    ReportHeader,
    ReportFooter,
    DetailHeader,
    Detail,
    DetailFooter,
    None
};

class OOoReportBuilder : public QObject
{
    Q_OBJECT
public:
    explicit OOoReportBuilder(QObject *parent = 0);

    void setArhiverFile(QString path) { m_arhiver = path; }
    void loadContentFile(QString file);
    void loadReportFile(QString filename);
    void showReport();
    void setOutputDir(QString dir) { m_outputDir = dir; }

    void appendModel(QAbstractItemModel* model);
    void appendParam(const QString name,const QVariant value);
    void setModels(QList<QAbstractItemModel*> models)
        { m_models = models; }
    void setParameters(QHash<QString,QVariant> params)
        { m_params = params; }

    void parse(bool show = true);
    ReportBands reportBand(QDomNode row);
    QString processString(const QString source);
    QString processWord(const QString source);
    QString processParams(QString paramName);

signals:

public slots:
    void finishedArh ( int exitCode, QProcess::ExitStatus exitStatus );


private:
    bool m_isLoaded;
    QString m_filename;
    QString m_fileContent;
    QString m_arhiver;
    QString m_outputDir;
    QString m_readyReportFile;
    QDomDocument m_doc;
    QHash<QString,QVariant> m_params;
    QList<QAbstractItemModel*> m_models;

    QString cellText(const QDomElement cell);
    void setText(const QDomElement &element, const QString text);
    void setText(const QDomElement &element, const double value);

    void saveContent();
    void saveReport();

    void parseDocument();
    void parseTable(QDomNode table);
    QDomElement parseRow(QDomNode row);

    QDomElement processDetail(const QDomElement &rowDetail);
    QString processFormula(const QString formula, int offset);



};

#endif // OOOREPORTBUILDER_H
