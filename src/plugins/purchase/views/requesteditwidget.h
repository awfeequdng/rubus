#ifndef REQUESTEDITWIDGET_H
#define REQUESTEDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"
#include "purchase_global.h"

class QSqlQueryModel;

namespace Ui {
class RequestEditWidget;
}

class PURCHASE_EXPORT  RequestEditWidget : public EditWidgetInterface
{
    Q_OBJECT
public:
    explicit RequestEditWidget(QWidget *parent = 0);
    ~RequestEditWidget();

    QVariant id() const;

    void setLocation(int location);

public slots:
    bool load(QVariant id);
    bool save();

private slots:
    void onItemChanged();
    void selectItem();

private:
    Ui::RequestEditWidget *ui;
    int m_id;

    QSqlQueryModel *m_equipmentModel;
    QSqlQueryModel *m_locationModel;
    QSqlQueryModel *m_itemModel;
    int m_location;

    void populate();
    int item();

    QString getUserInformation(QString role) const;

};

#endif // REQUESTEDITWIDGET_H

