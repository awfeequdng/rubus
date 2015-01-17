#ifndef REQUESTEDITDIALOG_H
#define REQUESTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class RequestEditDialog;
}

class RequestEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RequestEditDialog(QWidget *parent = 0);
    ~RequestEditDialog();

private:
    Ui::RequestEditDialog *ui;
};

#endif // REQUESTEDITDIALOG_H
