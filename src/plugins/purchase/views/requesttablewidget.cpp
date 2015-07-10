#include "requesttablewidget.h"
#include "ui_requesttablewidget.h"
#include "requesteditwidget.h"
#include "editdialog.h"
#include "advtableview.h"
#include "models/requestmodel.h"
#include "core.h"
#include "user.h"
#include "purchaseconstants.h"

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

    m_editWdg = new RequestEditWidget();
    m_editDlg = new EditDialog(m_editWdg, this);

    m_locationModel = new QSqlQueryModel(this);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
    connect(ui->ckHidden, SIGNAL(clicked()), SLOT(applyFilter()));
    connect(ui->ckPublic, SIGNAL(clicked()), SLOT(applyFilter()));
    connect(ui->ckOrdered, SIGNAL(clicked()), SLOT(applyFilter()));
    connect(ui->ckPaid, SIGNAL(clicked()), SLOT(applyFilter()));
    connect(ui->ckReceived, SIGNAL(clicked()), SLOT(applyFilter()));
    connect(ui->ckDelivered, SIGNAL(clicked()), SLOT(applyFilter()));
    connect(ui->cmbLocation, SIGNAL(keyChanged()), SLOT(applyFilter()));
    connect(ui->actionRefresh, SIGNAL(triggered()), SLOT(refresh()));

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
    qDebug() << "refresh";
    int location = ui->cmbLocation->currentKey().toInt();
    populate();
    ui->cmbLocation->setCurrentKey(location);
    applyFilter();
}

void RequestTableWidget::add()
{
    m_editWdg->setLocation(ui->cmbLocation->currentKey().toInt());

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

void RequestTableWidget::applyFilter()
{
    m_model->setLocation(ui->cmbLocation->currentKey().toInt());

    QSet<int> set;
    if (ui->ckPublic->isChecked()) {
        set.insert(Constants::STATE_PUBLIC);
    }
    if (ui->ckHidden->isChecked()) {
        set.insert(Constants::STATE_HIDDEN);
    }
    if (ui->ckOrdered->isChecked()) {
        set.insert(Constants::STATE_ORDERED);
    }
    if (ui->ckPaid->isChecked()) {
        set.insert(Constants::STATE_PAID);
    }
    if (ui->ckReceived->isChecked()) {
        set.insert(Constants::STATE_RECEIVED);
    }
    if (ui->ckDelivered->isChecked()) {
        set.insert(Constants::STATE_DELIVERED);
    }
    m_model->setStates(set);


    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }
}

void RequestTableWidget::showEvent(QShowEvent *e)
{
    populate();
    applyFilter();

    QWidget::showEvent(e);
}

void RequestTableWidget::populate()
{
    m_locationModel->setQuery(QString("SELECT lo_id, lo_name FROM locations WHERE lo_id IN (%1)")
                              .arg(Core::ICore::currentUser()->enabledLocations()));
    if (m_locationModel->lastError().isValid()) {
        qCritical() << m_locationModel->lastError();
        QMessageBox::critical(this, "Error", m_locationModel->lastError().text());
    }
    ui->cmbLocation->setModel(m_locationModel, 0, 1);
}
