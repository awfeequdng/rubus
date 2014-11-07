#include "itemeditwidget.h"
#include "ui_itemeditwidget.h"
#include "models/itemtypemodel.h"

#include <QtSql>

ItemEditWidget::ItemEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::ItemEditWidget),
    m_id(-1)
{
    ui->setupUi(this);

    m_typeModel = new ItemTypeModel(this);
    ui->cmbType->setModel(m_typeModel, ItemTypeModel::IdCol, ItemTypeModel::NameCol);

    m_unitModel = new QSqlQueryModel(this);

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
}

ItemEditWidget::~ItemEditWidget()
{
    delete ui;
}

bool ItemEditWidget::load(QVariant id)
{
    m_id = id.isValid() ? id.toInt() : -1;

    m_unitModel->setQuery(QString("SELECT un_id, un_name FROM units ORDER BY un_name"));
    if (m_unitModel->lastError().isValid()) {
        qCritical() << m_unitModel->lastError();
        setErrorString(m_unitModel->lastError().text());
        return false;
    }

    ui->cmbUnit->setModel(m_unitModel, 0, 1);

    ui->edName->setText(QString());
    ui->edDesc->setText(QString());
    ui->edArticle->setText(QString());
    ui->cmbUnit->setCurrentIndex(0);
    ui->cmbType->setCurrentIndex(0);
    ui->ckActive->setChecked(true);

    if (m_id != -1) {
        QSqlQuery sql;
        sql.exec(QString("SELECT it_name,it_desc,it_article,"
                         "it_unit,it_type,it_active FROM items "
                         "WHERE it_id = %1")
                 .arg(m_id));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qCritical() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
            setErrorString(tr("Item not found!"));
            return false;
        }

        ui->edName->setText(sql.value(0).toString());
        ui->edDesc->setText(sql.value(1).toString());
        ui->edArticle->setText(sql.value(2).toString());
        ui->cmbUnit->setCurrentKey(sql.value(3).toString());
        ui->cmbType->setCurrentKey(sql.value(4).toString());
        ui->ckActive->setChecked(sql.value(5).toBool());
    }

    return true;
}

bool ItemEditWidget::save()
{
    QSqlQuery sql;
    if (m_id == -1) {
        sql.prepare("INSERT INTO items(it_name,it_desc,it_article,"
                    "it_unit,it_type,it_active) "
                    "VALUES (:name,:desc,:article,"
                    ":unit,:type,:active)");
    } else {
        sql.prepare("UPDATE items SET it_name = :name,it_desc = :desc,it_article = :article,"
                    "it_unit = :unit,it_type = :type,it_active = :active "
                    "WHERE it_id = :id");
        sql.bindValue(":id", m_id);
    }

    sql.bindValue(":name", ui->edName->text());
    sql.bindValue(":desc", ui->edDesc->text());
    sql.bindValue(":article", ui->edArticle->text());
    sql.bindValue(":unit", ui->cmbUnit->currentKey().toString());
    sql.bindValue(":type", ui->cmbType->currentKey().toString());
    sql.bindValue(":active", ui->ckActive->isChecked());

    if (!sql.exec()) {
        qCritical() << sql.lastError();
        setErrorString(sql.lastError().text());
        QMessageBox::critical(this, "Error", sql.lastError().text());
        return false;
    }

    if (m_id == -1) {
        sql.exec("SELECT currval(pg_get_serial_sequence('items', 'it_id'))");

        if (sql.next()) {
            m_id = sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
        }
    }

    emit saved();
    return true;
}
