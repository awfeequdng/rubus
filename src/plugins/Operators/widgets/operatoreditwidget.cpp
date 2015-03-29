#include "operatoreditwidget.h"
#include "ui_operatoreditwidget.h"

#include <QtSql>
#include <QMessageBox>
#include "user.h"
#include "core.h"

OperatorEditWidget::OperatorEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::OperatorEditWidget),
    m_location(10031)
{
    ui->setupUi(this);

    m_jobModel = new QSqlQueryModel(this);
    m_shiftModel = new QSqlQueryModel(this);

    setFocusProxy(ui->edName);


    connect(ui->ckEducation, SIGNAL(toggled(bool)), ui->edEducation, SLOT(setEnabled(bool)));
    connect(ui->ckAttestation, SIGNAL(toggled(bool)), ui->edAttestation, SLOT(setEnabled(bool)));
    connect(ui->ckRemove, SIGNAL(toggled(bool)), ui->edRemove, SLOT(setEnabled(bool)));
    connect(ui->ckAttestation, SIGNAL(toggled(bool)), ui->edDown, SLOT(setEnabled(bool)));

    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(Core::ICore::instance(), SIGNAL(logged()), SLOT(getContractor()));

}

OperatorEditWidget::~OperatorEditWidget()
{
    delete ui;
}


QVariant OperatorEditWidget::id() const
{
    return m_id;
}

void OperatorEditWidget::setLocation(int location)
{
    m_location = location;
}

bool OperatorEditWidget::load(QVariant id)
{
    m_id = id.isValid() ? id.toInt() : -1;
    populate();


    if (m_id == -1) {
        ui->edName->setText(QString());
        ui->ckEducation->setChecked(true);
        ui->ckAttestation->setChecked(false);
        ui->ckRemove->setChecked(false);
        ui->edAttestator->setText(m_contractorName);
        m_contractorId = Core::ICore::currentUser()->contractorId();
        ui->edEducation->setDate(QDate::currentDate());
        ui->edAttestation->setDate(QDate::currentDate());
        ui->edRemove->setDate(QDate::currentDate());
        ui->edDown->setValue(0);
        setWindowTitle(tr("Add operator *"));
    } else {
        QSqlQuery sql;
        sql.exec(QString("SELECT oper.co_id as id, oper.co_name as name, op_date_begin_education, "
                 "op_date_attestation, op_remove_attestation, op_job, op_shift,"
                 "attest.co_name as attestator, oper.op_attestator as attestator_id, op_down "
                 "FROM operators as oper "
                 "JOIN contractors as attest ON attest.co_id = op_attestator "
                 "WHERE oper.co_id = %1").arg(m_id));
        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qDebug() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
            setErrorString(tr("Operator not found!"));
            return false;
        }

        ui->edName->setText(sql.value("name").toString());
        ui->cmbJob->setCurrentKey(sql.value("op_job").toInt());
        ui->cmbShift->setCurrentKey(sql.value("op_shift").toInt());

        QDate date = sql.value("op_date_begin_education").toDate();
        ui->ckEducation->setChecked(!date.isNull());
        ui->edEducation->setDate(date.isNull() ? QDate::currentDate() : date);

        date = sql.value("op_date_attestation").toDate();
        ui->ckAttestation->setChecked(!date.isNull());
        ui->edAttestation->setDate(date.isNull() ? QDate::currentDate() : date);

        date = sql.value("op_remove_attestation").toDate();
        ui->ckRemove->setChecked(!date.isNull());
        ui->edRemove->setDate(date.isNull() ? QDate::currentDate() : date);

        ui->edAttestator->setText(sql.value("attestator").toString());
        m_contractorId = sql.value("attestator_id").toInt();
        ui->edDown->setValue(sql.value("op_down").toInt());

        setWindowTitle(tr("Edit operator #%1").arg(m_id));
    }

    return true;
}

bool OperatorEditWidget::save()
{
    if (ui->edName->text().isEmpty()) {
        setErrorString(tr("Name is empty!"));
        QMessageBox::critical(this, "error", errorString());
        return false;
    }

    if (m_contractorId == -1) {
        setErrorString(tr("Contractor is not set!"));
        QMessageBox::critical(this, "error", errorString());
        return false;
    }

    QSqlQuery sql;
    if (m_id == -1) {
        sql.prepare("INSERT INTO operators (co_name, co_type, op_date_begin_education,"
                    "op_date_attestation,op_remove_attestation,op_attestator,"
                    "op_job, op_shift, op_down) VALUES (:name, :type, :date_begin_education,"
                    ":date_attestation,:remove_attestation,:attestator,"
                    ":job, :shift, :down)");
        sql.bindValue(":type", 10);
    } else {
        sql.prepare("UPDATE operators  SET co_name = :name, "
                    "op_date_begin_education = :date_begin_education,"
                    "op_date_attestation = :date_attestation,"
                    "op_remove_attestation = :remove_attestation,"
                    "op_attestator = :attestator,"
                    "op_job = :job, "
                    "op_shift = :shift, "
                    "op_down = :down "
                    "WHERE co_id = :id");
        sql.bindValue(":id", m_id);
    }

    sql.bindValue(":name", ui->edName->text().simplified());
    sql.bindValue(":date_begin_education", ui->ckEducation->isChecked() ? ui->edEducation->date() : QDate());
    sql.bindValue(":date_attestation", ui->ckAttestation->isChecked() ? ui->edAttestation->date() : QDate());
    sql.bindValue(":remove_attestation",ui->ckRemove->isChecked() ? ui->edRemove->date() : QDate());
    sql.bindValue(":attestator", m_contractorId);
    sql.bindValue(":job", ui->cmbJob->currentKey().toInt());
    sql.bindValue(":shift", ui->cmbShift->currentKey().toInt());
    sql.bindValue(":down", ui->edDown->value());

    if (!sql.exec()) {
        setErrorString(sql.lastError().text());
        qDebug() << sql.lastError();
        return false;
    }

    emit saved();
    return true;
}

void OperatorEditWidget::getContractor()
{
    QSqlQuery sql;

    sql.exec(QString("SELECT co_name FROM contractors WHERE co_id = %1").arg(Core::ICore::currentUser()->contractorId()));

    if (sql.lastError().isValid()) {
        qWarning() << sql.lastError();
        return;
    }

    if (!sql.next()) {
        qWarning() << "contractor not found! ";
        return;
    }

    m_contractorName = sql.value(0).toString();
}

void OperatorEditWidget::populate()
{
    m_jobModel->setQuery(QString("SELECT oj_id, oj_name FROM operator_jobs "
                         "WHERE oj_location = %1").arg(m_location));

    if (m_jobModel->lastError().isValid()) {
        qDebug() << m_jobModel->lastError();
    }

    m_shiftModel->setQuery(QString("SELECT sh_id, sh_name FROM shifts "
                                   "WHERE sh_location = %1").arg(m_location));

    if (m_shiftModel->lastError().isValid()) {
        qDebug() << m_shiftModel->lastError();
    }

    ui->cmbJob->setModel(m_jobModel, 0, 1);
    ui->cmbShift->setModel(m_shiftModel, 0, 1);
}
