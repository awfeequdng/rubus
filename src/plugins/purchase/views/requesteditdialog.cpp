#include "requesteditdialog.h"
#include "ui_requesteditdialog.h"

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
