#ifndef CONTRACTORTABLEDIALOG_H
#define CONTRACTORTABLEDIALOG_H

#include "widgets/tabledialog.h"

namespace Ui {
class ContractorTableDialog;
}

class ContractorModel;

class ContractorTableDialog : public TableDialog
{
    Q_OBJECT

public:
    explicit ContractorTableDialog(QWidget *parent = 0);
    ~ContractorTableDialog();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::ContractorTableDialog *ui;
    ContractorModel *m_model;
};

#endif // CONTRACTORTABLEDIALOG_H
