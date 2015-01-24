#include "requesteditwidget.h"
#include "ui_requesteditwidget.h"

#include <QtSql>
#include <QDebug>

RequestEditWidget::RequestEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::RequestEditWidget)
{
    ui->setupUi(this);

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
}

RequestEditWidget::~RequestEditWidget()
{
    delete ui;
}


QVariant RequestEditWidget::id() const
{
    return m_id;
}

bool RequestEditWidget::load(QVariant id)
{
    m_id = id.isValid() ? id.toInt() : -1;

    if (m_id != -1) {
        QSqlQuery sql;
        sql.exec(QString("SELECT re_state,re_item,re_equipment_type,"
                         "re_user, re_date, re_qty, "
                         "re_balance, re_storage, re_note ,"
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

        ui->edState->setText("Private");
        ui->cmbStorage->setCurrentKey(sql.value("re_storage"));
        ui->cmbEquipment->setCurrentKey(sql.value("re_equipment_type"));
        ui->cmbItem->setCurrentKey(sql.value("re_item"));
        ui->edQty->setValue(sql.value("re_qty").toDouble());
        ui->edBalance->setValue(sql.value("re_balance").toDouble());
        ui->edNote->setText(sql.value("re_note").toString());
        ui->labUser->setText(sql.value("re_user").toString()); // TODO Look user information
        ui->labUnit->setText(sql.value("un_name").toString());
    }

    ui->edId->setText(QString::number(m_id));

    setWindowTitle(m_id == -1 ? tr("New item") : tr("Edit  item #%1").arg(m_id));
    return true;
}

bool RequestEditWidget::save()
{
    QSqlQuery sql;

    if (m_id == -1) {
        sql.prepare("INSERT INTO requests (re_state,re_item,re_equipment_type,"
                    "re_user, re_date, re_qty, "
                    "re_balance, re_storage, re_note) VALUES (:state,:item,:equipment_type,"
                    "USER, CURRENT_DATE, :qty, "
                    ":balance, :storage, :note)");
    } else {
        sql.prepare("UPDATE requests SET "
                    "re_state = :state,"
                    "re_item = :item,"
                    "re_equipment_type = :equipment_type,"
                    "re_qty = :qty, "
                    "re_balance = :balance, "
                    "re_storage = :storage, "
                    "re_note = :note "
                    "WHERE re_id = :id");
        sql.bindValue(":id", m_id);
    }

    if (!sql.exec()) {
        setErrorString(sql.lastError().text());
        qCritical() << sql.lastError();
        return false;
    }

    if (m_id == -1) {
        sql.exec("SELECT currval(pg_get_serial_sequence('requests', 're_id'))");

        if (sql.next()) {
            m_id = sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
        }
    }

    emit saved();
    return true;
}

