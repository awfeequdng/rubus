#ifndef REQUESTTABLEWIDGET_H
#define REQUESTTABLEWIDGET_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class RequestTableWidget;
}
class RequestModel;

class RequestTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestTableWidget(QWidget *parent = 0);
    ~RequestTableWidget();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::RequestTableWidget *ui;
    RequestModel *m_model;
};

#endif // REQUESTTABLEWIDGET_H
