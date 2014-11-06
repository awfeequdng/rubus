#include "standardtabledialog.h"
#include "ui_standardtabledialog.h"

StandardTableDialog::StandardTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StandardTableDialog)
{
    ui->setupUi(this);
}

StandardTableDialog::~StandardTableDialog()
{
    delete ui;
}
