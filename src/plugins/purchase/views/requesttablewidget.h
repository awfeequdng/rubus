#ifndef REQUESTTABLEWIDGET_H
#define REQUESTTABLEWIDGET_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class RequestTableWidget;
}
class RequestModel;
class RequestEditWidget;
class EditDialog;
class Controller;

class RequestTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestTableWidget(QWidget *parent = 0);
    ~RequestTableWidget();

    int currentId() const;
    void refresh();

public slots:
    void add();
    void editCurrent();
    void deleteSelected();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::RequestTableWidget *ui;
    RequestModel *m_model;
    EditDialog *m_editDlg;

    Controller *m_controller;
};

#endif // REQUESTTABLEWIDGET_H
