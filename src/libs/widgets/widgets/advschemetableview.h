#ifndef ADVSCHEMETABLEVIEW_H
#define ADVSCHEMETABLEVIEW_H

#include "advtableview.h"
#include "widgets_global.h"

class WIDGETS_EXPORT AdvSchemeTableView : public AdvTableView
{
    Q_OBJECT
public:
    explicit AdvSchemeTableView(QWidget *parent = 0);

    void setScheme(const QString &json);
    QString scheme() const;

    void setSchemeObject(const QString &objectName);
    QString schemeObject() const;

signals:

public slots:

private:
    QString m_scheme;
    QString m_schemeObject;
    QStringList m_columns;

private:
    void parseScheme(const QString &json, const QString &objectName);

};

#endif // ADVSCHEMETABLEVIEW_H
