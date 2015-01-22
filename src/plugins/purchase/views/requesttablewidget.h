#ifndef REQUESTTABLEWIDGET_H
#define REQUESTTABLEWIDGET_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class RequestTableWidget;
}
class RequestModel;
class RequestEditDialog;
class Controller;

class RequestTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestTableWidget(QWidget *parent = 0);
    ~RequestTableWidget();


public slots:
    void add();
    void editCurrent();
    void deleteSelected();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::RequestTableWidget *ui;
    RequestModel *m_model;
    RequestEditDialog *m_editDlg;
    Controller *m_controller;
};

#endif // REQUESTTABLEWIDGET_H
