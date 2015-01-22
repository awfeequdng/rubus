#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "requesteditdialog.h"

#include "advschemetableview.h"
#include "models/requestmodel.h"
#include "core.h"
#include "controller.h"

#include <QMessageBox>

RequestTableWidget::RequestTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestTableWidget)
{
    ui->setupUi(this);
    m_model = new RequestModel(this);
    m_model->setScheme(Core::ICore::schemeSource());
    m_model->setSchemeObject("Request");

    ui->tableView->setScheme(Core::ICore::schemeSource());
    ui->tableView->setSchemeObject("Request");


    m_editDlg = new RequestEditDialog(this);
    m_controller = new Controller(this);
    m_controller->setSchemeObject("Request");
    m_editDlg->setController(m_controller);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
}

RequestTableWidget::~RequestTableWidget()
{
    delete ui;
}

void RequestTableWidget::add()
{
    if (!m_controller->load()) {
        QMessageBox::critical(this, tr("Error"), m_controller->errorString());
        return;
    }

    m_editDlg->exec();
}

void RequestTableWidget::editCurrent()
{
    if (!m_controller->load()) {
        QMessageBox::critical(this, tr("Error"), m_controller->errorString());
        return;
    }

    m_editDlg->exec();
}

void RequestTableWidget::deleteSelected()
{

}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }
    ui->tableView->setModel(m_model);
    QWidget::showEvent(e);
}
