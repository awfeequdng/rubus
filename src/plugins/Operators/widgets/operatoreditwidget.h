#ifndef OPERATOREDITWIDGET_H
#define OPERATOREDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class OperatorEditWidget;
}

class QSqlQueryModel;

class OperatorEditWidget : public EditWidgetInterface
{
    Q_OBJECT

public:
    explicit OperatorEditWidget(QWidget *parent = 0);
    ~OperatorEditWidget();

    QVariant id() const;
    void setLocation(int location);

public slots:
    bool load(QVariant id);
    bool save();

private slots:
    void getContractor();

private:
    Ui::OperatorEditWidget *ui;

    int m_id;
    int m_location;
    int m_contractorId;
    QString m_contractorName;
    QSqlQueryModel *m_jobModel;
    QSqlQueryModel *m_shiftModel;

    void populate();


};

#endif // OPERATOREDITWIDGET_H
