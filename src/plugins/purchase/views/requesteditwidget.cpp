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
#include "chooseitemdialog.h"

#include <QtSql>
#include <QDebug>
#include <QMessageBox>
#include <QDirModel>

RequestEditWidget::RequestEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::RequestEditWidget),
    m_chooseItemDialog(0),
    m_location(-1),
    m_item(-1),
    m_eqType(-1)
{
    ui->setupUi(this);

    m_equipmentModel = new EquipmentTypeModel(this);
    m_locationModel = new QSqlQueryModel(this);
    m_itemModel = new QSqlQueryModel(this);


    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
    connect(ui->btnSelectItem, SIGNAL(clicked()), SLOT(chooseItem()));
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
                         "re_balance, re_location, re_note "
                         "FROM requests "
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
        m_item = sql.value("re_item").toInt();
        m_eqType = sql.value("re_equipment_type").toInt();
        ui->edQty->setValue(sql.value("re_qty").toDouble());
        ui->edBalance->setValue(sql.value("re_balance").toDouble());
        ui->edNote->setText(sql.value("re_note").toString());
        ui->labUser->setText(getUserInformation(sql.value("re_user").toString()));
    } else {
        ui->labUser->setText(getUserInformation(Core::ICore::currentUser()->rolename()));
        ui->cmbLocation->setCurrentKey(m_location);
        m_item = -1;
        m_eqType = -1;
    }

    onItemChanged();

    ui->edId->setText(QString::number(m_id));


    setWindowTitle(m_id == -1 ? tr("New item") : tr("Edit  item #%1").arg(m_id));
    return true;
}

bool RequestEditWidget::save()
{
    if (item() == -1) {
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
    sql.bindValue(":item", item());
    sql.bindValue(":equipment_type", m_equipmentModel->index(ui->cmbEquipment->currentIndex(),
                                                             0,
                                                             ui->cmbEquipment->rootModelIndex()).data(Qt::EditRole).toInt());
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
    ui->labUnit->clear();
    ui->edItem->clear();

    if (m_item < 0) {
        return;
    }

    QSqlQuery sql;
    sql.exec(QString("SELECT it_name, it_article, un_name FROM items "
             "JOIN units ON un_id = it_unit "
             "WHERE it_id = %1").arg(m_item));

    if (sql.lastError().isValid()) {
        qDebug() << sql.lastError();
        return;
    }

    if (!sql.next()) {
        qDebug() << "Can't find item id " << m_item;
    }

    QString name = sql.value(0).toString();
    if (!sql.value(1).toString().simplified().isEmpty()){
        name.append(" (" + sql.value(1).toString() + ")");
    }
    ui->edItem->setText(name);
    ui->labUnit->setText(sql.value(2).toString());

    QModelIndex index = m_equipmentModel->indexById(m_eqType);
    if (index.isValid()) {
        ui->cmbEquipment->setRootModelIndex(index.parent());
        ui->cmbEquipment->setCurrentIndex(index.row());
    }
}

void RequestEditWidget::chooseItem()
{
    if (!m_chooseItemDialog) {
        m_chooseItemDialog = new ChooseItemDialog(this);
    }

    if (m_chooseItemDialog->exec(m_item) == QDialog::Accepted)  {
        m_item = m_chooseItemDialog->selectedItemId();
        onItemChanged();
    }

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
}

int RequestEditWidget::item()
{
    return m_item;
}

QString RequestEditWidget::getUserInformation(QString role) const
{
    QSqlQuery sql;
    sql.exec(QString("SELECT up_name FROM user_params WHERE up_role = '%1'").arg(role));

    sql.next();
    return sql.value(0).toString();
}

