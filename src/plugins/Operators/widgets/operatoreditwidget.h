#ifndef OPERATOREDITWIDGET_H
#define OPERATOREDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class OperatorEditWidget;
}

class OperatorEditWidget : public EditWidgetInterface
{
    Q_OBJECT

public:
    explicit OperatorEditWidget(QWidget *parent = 0);
    ~OperatorEditWidget();

    QVariant id() const;

public slots:
    bool load(QVariant id);
    bool save();

private:
    Ui::OperatorEditWidget *ui;

};

#endif // OPERATOREDITWIDGET_H
