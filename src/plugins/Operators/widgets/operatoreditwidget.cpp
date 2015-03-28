#include "operatoreditwidget.h"
#include "ui_operatoreditwidget.h"

OperatorEditWidget::OperatorEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::OperatorEditWidget)
{
    ui->setupUi(this);
}

OperatorEditWidget::~OperatorEditWidget()
{
    delete ui;
}


QVariant OperatorEditWidget::id() const
{
    return 0;
}

bool OperatorEditWidget::load(QVariant id)
{
    return true;
}

bool OperatorEditWidget::save()
{
    return true;
}
