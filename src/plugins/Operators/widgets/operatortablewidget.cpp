#include "operatortablewidget.h"

#include <QIcon>

OperatorTableWidget::OperatorTableWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowIcon(QIcon(":/images/operator.png"));
}
