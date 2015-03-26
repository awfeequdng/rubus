#include "operatortablewidget.h"
#include "ui_operatortablewidget.h"

#include "models/operatormodel.h"

OperatorTableWidget::OperatorTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperatorTableWidget)
{
    ui->setupUi(this);

    m_model = new OperatorModel(this);
}

OperatorTableWidget::~OperatorTableWidget()
{
    delete ui;
}
