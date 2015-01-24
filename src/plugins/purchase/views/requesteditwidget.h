#ifndef REQUESTEDITWIDGET_H
#define REQUESTEDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class RequestEditWidget;
}

class RequestEditWidget : public EditWidgetInterface
{
    Q_OBJECT
public:
    explicit RequestEditWidget(QWidget *parent = 0);
    ~RequestEditWidget();

    QVariant id() const;

public slots:
    bool load(QVariant id);
    bool save();
private:
    Ui::RequestEditWidget *ui;
    int m_id;

};

#endif // REQUESTEDITWIDGET_H

