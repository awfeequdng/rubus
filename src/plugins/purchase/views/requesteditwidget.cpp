#include "requesteditwidget.h"
#include "ui_requesteditwidget.h"
#include "user.h"
#include "core.h"
#include "purchaseconstants.h"
#include "models/requestmodel.h"
#include "../../plugins/base/baseplugin.h"
#include "../../plugins/base/item.h"
#include "treecombobox.h"
#include "models/equipmenttypemodel.h"

#include <QtSql>
#include <QDebug>
#include <QMessageBox>
#include <QDirModel>

RequestEditWidget::RequestEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::RequestEditWidget)
{
    ui->setupUi(this);

    m_equipmentModel = new EquipmentTypeModel(this);
    m_locationModel = new QSqlQueryModel(this);
    m_itemModel = new QSqlQueryModel(this);
    m_location = -1;

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
    connect(ui->cmbItem, SIGNAL(keyChanged()), SLOT(onItemChanged()));
    connect(ui->btnSelectItem, SIGNAL(clicked()), SLOT(selectItem()));
}

RequestEditWidget::~RequestEditWidget()
{
    delete ui;
}


QVariant RequestEditWidget::id() const
{
    return m_id;
}

void RequestEditWidget::setLocation(int location)
{
    m_location = location;
}

bool RequestEditWidget::load(QVariant id)
{
    populate();
    m_id = id.isValid() ? id.toInt() : -1;

    if (m_id != -1) {
        QSqlQuery sql;
        sql.exec(QString("SELECT re_state,re_item,re_equipment_type,"
                         "re_user, re_date, re_qty, "
                         "re_balance, re_location, re_note ,"
                         "un_name "
                         "FROM requests "
                         "JOIN items ON re_item = it_id "
                         "JOIN units ON it_unit = un_id "
                         "WHERE re_id = %1")
                 .arg(m_id));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qCritical() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
            setErrorString(tr("Item #%1 not found!").arg(m_id));
            return false;
        }

        ui->edState->setText(RequestModel::stateName(sql.value("re_state").toInt()));
        ui->cmbLocation->setCurrentKey(sql.value("re_location"));
        //ui->cmbEquipment->setCurrentKey(sql.value("re_equipment_type"));
        ui->cmbItem->setCurrentKey(sql.value("re_item"));
        ui->edQty->setValue(sql.value("re_qty").toDouble());
        ui->edBalance->setValue(sql.value("re_balance").toDouble());
        ui->edNote->setText(sql.value("re_note").toString());
        ui->labUser->setText(getUserInformation(sql.value("re_user").toString()));
        ui->labUnit->setText(sql.value("un_name").toString());
    } else {
        ui->labUser->setText(getUserInformation(Core::ICore::currentUser()->rolename()));
        //ui->cmbLocation->setCurrentKey(m_location);
        ui->labUnit->clear();
    }

    ui->edId->setText(QString::number(m_id));


    setWindowTitle(m_id == -1 ? tr("New item") : tr("Edit  item #%1").arg(m_id));
    return true;
}

bool RequestEditWidget::save()
{
    int it_id = item();
    if (it_id == -1) {
        return false;
    }

    QSqlQuery sql;

    if (m_id == -1) {
        sql.prepare("INSERT INTO requests (re_state,re_item,re_equipment_type,"
                    "re_user, re_date, re_qty, "
                    "re_balance, re_location, re_note) VALUES (:state,:item,:equipment_type,"
                    "USER, CURRENT_DATE, :qty, "
                    ":balance, :location, :note)");
    } else {
        sql.prepare("UPDATE requests SET "
                    "re_state = :state,"
                    "re_item = :item,"
                    "re_equipment_type = :equipment_type,"
                    "re_qty = :qty, "
                    "re_balance = :balance, "
                    "re_location = :location, "
                    "re_note = :note "
                    "WHERE re_id = :id");
        sql.bindValue(":id", m_id);
    }

    sql.bindValue(":state", Constants::STATE_HIDDEN);
    sql.bindValue(":item", it_id);
    //sql.bindValue(":equipment_type",ui->cmbEquipment->currentKey().toInt());
    sql.bindValue(":qty", ui->edQty->value());
    sql.bindValue(":balance", ui->edBalance->value());
    sql.bindValue(":location",ui->cmbLocation->currentKey().toInt());
    sql.bindValue(":note",ui->edNote->toPlainText());

    if (!sql.exec()) {
        setErrorString(sql.lastError().text());
        qCritical() << sql.lastError();
        QMessageBox::critical(this, "Error", sql.lastError().text());
        return false;
    }

    if (m_id == -1) {
        sql.exec("SELECT currval(pg_get_serial_sequence('requests', 're_id'))");

        if (sql.next()) {
            m_id = sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
            QMessageBox::warning(this, "Error", sql.lastError().text());
        }
    }

    emit saved();
    return true;
}

void RequestEditWidget::onItemChanged()
{
    if (ui->cmbItem->currentKey().toInt() > 0) {
        ui->labUnit->setText(m_itemModel->index(ui->cmbItem->currentIndex(), 2).data().toString());
    }
}

void RequestEditWidget::selectItem()
{
}

void RequestEditWidget::populate()
{
    m_locationModel->setQuery(QString("SELECT lo_id, lo_name FROM locations "
                              "WHERE lo_id IN (%1)")
                              .arg(Core::ICore::currentUser()->enabledLocations()));

    if (m_locationModel->lastError().isValid()) {
        qCritical() << m_locationModel->lastError();
        QMessageBox::critical(this, "Error", m_locationModel->lastError().text());
    }

    ui->cmbLocation->setModel(m_locationModel, 0, 1);


    m_equipmentModel->populate();
    ui->cmbEquipment->setModel(m_equipmentModel);

    m_itemModel->setQuery("SELECT it_id, it_name || "
                          "CASE it_article  WHEN '' THEN '' "
                          "ELSE ' (art.:' || it_article || ')' END AS it_name, un_name "
                          "FROM items "
                          "JOIN units ON un_id = it_unit "
                          "ORDER BY it_name");

    if (m_itemModel->lastError().isValid()) {
        qCritical() << m_itemModel->lastError();
        QMessageBox::critical(this, "Error", m_itemModel->lastError().text());
    }

    ui->cmbItem->setModel(m_itemModel, 0, 1);
}

int RequestEditWidget::item()
{
    if (ui->cmbItem->currentText().compare(m_itemModel->index(ui->cmbItem->currentIndex(), 1)
                                           .data().toString().simplified(), Qt::CaseInsensitive) != 0) {
        QSqlQuery sql;
        sql.exec(QString("INSERT INTO items(it_name, it_unit, it_type, it_active, it_article) VALUES ('%1', 'EA', 'P', true, '')")
                 .arg(ui->cmbItem->currentText().simplified()));

        if (sql.lastError().isValid()) {
            qCritical() << m_itemModel->lastError();
            QMessageBox::critical(this, "Error", sql.lastError().text());
            return -1;
        }

        sql.exec("SELECT currval(pg_get_serial_sequence('items', 'it_id'))");

        if (sql.next()) {
            return sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
            QMessageBox::warning(this, "Error", sql.lastError().text());
            return -1;
        }
    }

    return ui->cmbItem->currentKey().toInt();
}

QString RequestEditWidget::getUserInformation(QString role) const
{
    QSqlQuery sql;
    sql.exec(QString("SELECT up_name FROM user_params WHERE up_role = '%1'").arg(role));

    sql.next();
    return sql.value(0).toString();
}

