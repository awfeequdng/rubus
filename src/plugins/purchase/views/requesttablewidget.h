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

class RequestTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestTableWidget(QWidget *parent = 0);
    ~RequestTableWidget();

    int currentId() const;

public slots:
    void add();
    void editCurrent();
    void deleteSelected();
    void refresh();

private slots:
    void applyFilter();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::RequestTableWidget *ui;
    RequestModel *m_model;
    EditDialog *m_editDlg;
    RequestEditWidget *m_editWdg;
    QSqlQueryModel *m_locationModel;

    void populate();


};

#endif // REQUESTTABLEWIDGET_H
