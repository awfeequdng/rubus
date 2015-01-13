#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "advtableview.h"

RequestTableWidget::RequestTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestTableWidget)
{
    ui->setupUi(this);
    m_model = new QSqlQueryModel(this);
}

RequestTableWidget::~RequestTableWidget()
{
    delete ui;
}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    m_model->setQuery("SELECT * FROm requests");
    ui->tableView->setModel(m_model);
    QWidget::showEvent(e);
}
