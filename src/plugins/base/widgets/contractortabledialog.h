#ifndef CONTRACTORTABLEDIALOG_H
#define CONTRACTORTABLEDIALOG_H

#include "widgets/tabledialog.h"
#include "report.h"

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

public slots:
    void slotPrint(Report &r);

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::ContractorTableDialog *ui;
    ContractorModel *m_model;
};

#endif // CONTRACTORTABLEDIALOG_H
