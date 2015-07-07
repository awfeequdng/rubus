#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "requesteditwidget.h"
#include "editdialog.h"
#include "advtableview.h"
#include "models/requestmodel.h"
#include "core.h"

#include <QMessageBox>
#include <QDebug>

RequestTableWidget::RequestTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestTableWidget)
{
    ui->setupUi(this);
    m_model = new RequestModel(this);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::IdCol, 60);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::StateCol, 100);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::DateCol, 120);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::StorageCol, 120);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::EquipmentCol, 120);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::QtyCol, 80);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(RequestModel::BalanceCol, 80);
    ui->tableView->setModel(m_model);

    m_editDlg = new EditDialog(new RequestEditWidget(), this);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
}

RequestTableWidget::~RequestTableWidget()
{
    delete ui;
}

int RequestTableWidget::currentId() const
{
    return ui->tableView->currentId().toInt();
}

void RequestTableWidget::refresh()
{
    m_model->populate();
}

void RequestTableWidget::add()
{
    if (m_editDlg->exec() == QDialog::Accepted) {
        refresh();
    }
}

void RequestTableWidget::editCurrent()
{
    if (m_editDlg->exec(currentId()) == QDialog::Accepted) {
        refresh();
    }
}

void RequestTableWidget::deleteSelected()
{

}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }

    QWidget::showEvent(e);
}
