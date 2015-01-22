#include "requesteditdialog.h"
#include "ui_requesteditdialog.h"
#include "controller.h"

#include <QMessageBox>

RequestEditDialog::RequestEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RequestEditDialog)
{
    ui->setupUi(this);    
}

RequestEditDialog::~RequestEditDialog()
{
    delete ui;
}

void RequestEditDialog::setController(Controller *controller)
{
    m_controller = controller;
    m_controller->bindData(ui->edQty, "value", "qty");
    m_controller->bindData(ui->cmbStorage, "");
}

void RequestEditDialog::accept()
{
    if (!m_controller->save()) {
        QMessageBox::critical(this, tr("Error"), m_controller->errorString());
        return;
    }

    QDialog::accept();
}
