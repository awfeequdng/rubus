#ifndef REQUESTEDITDIALOG_H
#define REQUESTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class RequestEditDialog;
}

class Controller;

class RequestEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RequestEditDialog(QWidget *parent = 0);
    ~RequestEditDialog();

    void setController(Controller *controller);

public slots:
    void accept();

private:
    Ui::RequestEditDialog *ui;
    Controller *m_controller;
};

#endif // REQUESTEDITDIALOG_H
