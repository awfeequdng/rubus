#ifndef OPERATORTABLEWIDGET_H
#define OPERATORTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class OperatorTableWidget;
}

class OperatorModel;

class OperatorTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperatorTableWidget(QWidget *parent = 0);
    ~OperatorTableWidget();

private:
    Ui::OperatorTableWidget *ui;

    OperatorModel *m_model;
};

#endif // OPERATORTABLEWIDGET_H
