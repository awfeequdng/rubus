#ifndef REQUESTEDITWIDGET_H
#define REQUESTEDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"
#include "purchase_global.h"

class QSqlQueryModel;
class EquipmentTypeModel;
class ChooseItemDialog;

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
    void chooseItem();

private:
    Ui::RequestEditWidget *ui;
    int m_id;

    EquipmentTypeModel *m_equipmentModel;
    QSqlQueryModel *m_locationModel;
    QSqlQueryModel *m_itemModel;
    ChooseItemDialog *m_chooseItemDialog;
    int m_location;
    int m_item;
    int m_eqType;

    void populate();
    int item();

    QString getUserInformation(QString role) const;

};

#endif // REQUESTEDITWIDGET_H

