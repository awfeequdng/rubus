#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "advtableview.h"
#include "models/requestmodel.h"
#include "core.h"

RequestTableWidget::RequestTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestTableWidget)
{
    ui->setupUi(this);
    m_model = new RequestModel(this);
    m_model->setScheme(Core::ICore::scheme());
    m_model->setSchemeObject("Request");
}

RequestTableWidget::~RequestTableWidget()
{
    delete ui;
}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    ui->tableView->setModel(m_model);
    QWidget::showEvent(e);
}
