#include "contractortabledialog.h"
#include "ui_contractortabledialog.h"
#include "models/contractormodel.h"

ContractorTableDialog::ContractorTableDialog(QWidget *parent) :
    TableDialog(parent),
    ui(new Ui::ContractorTableDialog)
{
    ui->setupUi(this);

    m_model = new ContractorModel(this);
    setModel(m_model, ContractorModel::IdCol, Qt::DisplayRole);

    setView(ui->tableView);

    restoreSettings();
}

ContractorTableDialog::~ContractorTableDialog()
{
    saveSettings();
    delete ui;
}
