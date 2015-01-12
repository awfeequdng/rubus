#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "qadvancedtableview.h"

RequestTableWidget::RequestTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestTableWidget)
{
    ui->setupUi(this);
    m_model = new QSqlQueryModel(this);

//    ui->tableView->setAlternatingRowColors(true);
//    ui->tableView->verticalHeader()->setDefaultSectionSize(30);
//    ui->tableView->horizontalHeader()->setVisible(true);
//    ui->tableView->setShowFilter(false);

}

RequestTableWidget::~RequestTableWidget()
{
    delete ui;
}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    m_model->setQuery("SELECT * FROm contractors");
    ui->tableView->setModel(m_model);
    ui->tableView->setShowFilter(false);
    ui->tableView->horizontalHeader()->setc
    QWidget::showEvent(e);
}
