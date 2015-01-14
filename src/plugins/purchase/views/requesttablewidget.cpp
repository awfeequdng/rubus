#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "advschemetableview.h"
#include "models/requestmodel.h"
#include "core.h"

#include <QMessageBox>

RequestTableWidget::RequestTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestTableWidget)
{
    ui->setupUi(this);
    m_model = new RequestModel(this);
    m_model->setScheme(Core::ICore::scheme());
    m_model->setSchemeObject("Request");

    ui->tableView->setScheme(Core::ICore::scheme());
    ui->tableView->setSchemeObject("Request");
}

RequestTableWidget::~RequestTableWidget()
{
    delete ui;
}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }
    ui->tableView->setModel(m_model);
    QWidget::showEvent(e);
}
