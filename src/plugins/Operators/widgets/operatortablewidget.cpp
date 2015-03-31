#include "operatortablewidget.h"
#include "ui_operatortablewidget.h"
#include "operatoreditwidget.h"
#include "models/operatormodel.h"
#include "widgets/editdialog.h"

#include <QSortFilterProxyModel>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QtSql>
#include <QTextCodec>
#include "user.h"
#include "core.h"

OperatorTableWidget::OperatorTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperatorTableWidget)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/operator.png"));

    m_editWidget = new OperatorEditWidget();
    m_editDlg = new EditDialog(m_editWidget, this);

    m_locationModel = new QSqlQueryModel(this);
    m_jobModel = new QSqlQueryModel(this);
    m_shiftModel = new QSqlQueryModel(this);

    m_model = new OperatorModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(OperatorModel::NameCol);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::IdCol, 50);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::DownCol, 50);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::BeginEducCol, 100);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::DateAttestCol, 100);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::DateRemoveCol, 100);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::JobCol, 150);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::ShiftCol, 150);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(OperatorModel::AttestatorCol, 150);
    ui->tableView->horizontalHeader()->setSectionHidden(OperatorModel::IdCol, true);
    ui->tableView->setModel(m_proxyModel, OperatorModel::IdCol);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionsClickable(true);
    ui->tableView->horizontalHeader()->setSectionsMovable(true);

    m_acAdd = new QAction(tr("Add"), this);
    m_acAdd->setShortcut(Qt::Key_Insert);
    connect(m_acAdd, SIGNAL(triggered()), SLOT(add()));

    m_acEdit = new QAction(tr("Edit"), this);
    connect(m_acEdit, SIGNAL(triggered()), SLOT(editCurrent()));

    m_acRemove = new QAction(tr("Delete"), this);
    m_acRemove->setShortcut(QKeySequence::Delete);
    connect(m_acRemove, SIGNAL(triggered()), SLOT(removeSelected()));

    ui->tableView->addAction(m_acAdd);
    ui->tableView->addAction(m_acEdit);
    ui->tableView->addAction(m_acRemove);
    addAction(m_acAdd);
    addAction(m_acEdit);
    addAction(m_acRemove);

    //-----------
    m_export = new QAction(tr("Export"),this);
    connect(m_export, SIGNAL(triggered()), SLOT(export32()));
    addAction(m_export);
    //-----------

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(editCurrent()));
    connect(ui->edFind, SIGNAL(textEdited(QString)), m_proxyModel, SLOT(setFilterRegExp(QString)));
    connect(ui->tabs, SIGNAL(currentChanged(int)), SLOT(filter()));
    connect(ui->cmbLocation, SIGNAL(keyChanged()), SLOT(filter()));
    connect(ui->cmbShift, SIGNAL(keyChanged()), SLOT(filter()));
    connect(ui->cmbJob, SIGNAL(keyChanged()), SLOT(filter()));

    QSettings sett;
    ui->tableView->restoreHeaderGeometry(sett.value("OperatorTableWidget/view").toByteArray());
    ui->tableView->horizontalHeader()->restoreState(sett.value("OperatorTableWidget/headerState").toByteArray());
}

OperatorTableWidget::~OperatorTableWidget()
{
    QSettings sett;
    sett.setValue("OperatorTableWidget/view", ui->tableView->saveHeaderGeometry());
    sett.setValue("OperatorTableWidget/headerState", ui->tableView->horizontalHeader()->saveState());

    delete ui;
}

void OperatorTableWidget::showEvent(QShowEvent *e)
{
    popalateLocation();
    populate();
    filter();

    QWidget::showEvent(e);
}

int OperatorTableWidget::currentId() const
{
    return ui->tableView->currentId().toInt();
}

int OperatorTableWidget::currentLocation() const
{
    return ui->cmbLocation->currentKey().toInt();
}

void OperatorTableWidget::add()
{
    m_editWidget->setLocation(currentLocation());
    if (m_editDlg->exec()) {
        filter();
        ui->tableView->setCurrentId(m_editWidget->id());
    }
}

void OperatorTableWidget::editCurrent()
{
    m_editWidget->setLocation(currentLocation());
    if (m_editDlg->exec(currentId())) {
        filter();
        ui->tableView->setCurrentId(m_editWidget->id());
    }
}

void OperatorTableWidget::removeSelected()
{
    int count = ui->tableView->selectionModel()->selectedRows().count();
    if (count <= 0) {
        return;
    }

    if (QMessageBox::warning(this, "Delete", tr("Remove %n operators?", "", count), QMessageBox::Yes | QMessageBox::No)
            == QMessageBox::Yes) {
        QSqlQuery sql;
        sql.exec(QString("DELETE FROM operators WHERE co_id IN (%1)").arg(selectedIds()));

        if (sql.lastError().isValid()) {
            qCritical() << sql.lastError();
            QMessageBox::critical(this, "Error", sql.lastError().text());
            return;
        }

        filter();
    }
}

void OperatorTableWidget::populate()
{
    m_jobModel->setQuery(QString("SELECT -1 as oj_id, '%2' as oj_name UNION "
                                 "SELECT oj_id, oj_name FROM operator_jobs WHERE oj_location = %1")
                         .arg(currentLocation())
                         .arg(tr("-- All --")));

    if (m_jobModel->lastError().isValid()) {
        qCritical() << m_jobModel->lastError();
        QMessageBox::critical(this, "Error", m_jobModel->lastError().text());
    }

    ui->cmbJob->setModel(m_jobModel, 0 ,1);

    m_shiftModel->setQuery(QString("SELECT -1 as sh_id, '%2' as sh_name UNION "
                               "SELECT sh_id, sh_name FROM shifts WHERE sh_location = %1")
                           .arg(currentLocation())
                           .arg(tr("-- All --")));

    if (m_shiftModel->lastError().isValid()) {
        qCritical() << m_shiftModel->lastError();
        QMessageBox::critical(this, "Error", m_shiftModel->lastError().text());
    }

    ui->cmbShift->setModel(m_shiftModel, 0, 1);
}

void OperatorTableWidget::popalateLocation()
{
    QJsonArray arr = Core::ICore::currentUser()->parameterValue("locations").toArray();
    QListIterator<QVariant> i(arr.toVariantList());
    QString enabledLocations;

    while (i.hasNext()) {
        if (!enabledLocations.isEmpty()) {
            enabledLocations.append(",");
        }
        enabledLocations.append(i.next().toString());
    }

    m_locationModel->setQuery(QString("SELECT lo_id, lo_name FROM locations "
                                      "WHERE lo_id IN (%1)").arg(enabledLocations));

    if (m_locationModel->lastError().isValid()) {
        qDebug() << m_locationModel->lastError();
        QMessageBox::critical(this, "Error", m_locationModel->lastError().text());
    }

    ui->cmbLocation->setModel(m_locationModel, 0, 1);
}

void OperatorTableWidget::export32()
{
    QSqlQuery sql;
    sql.prepare("INSERT INTO operators (co_name,co_type,op_date_begin_education,"
            "op_date_attestation,op_remove_attestation,op_attestator,"
            "op_job,op_shift,op_down) VALUES (:name,10,:date_begin_education,"
            ":date_attestation,:remove_attestation,:attestator,"
            ":job,:shift,:down)");

    QFile f("c:/operators.txt");
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << f.errorString();
        return;
    }

    QTextStream stream(&f);
    stream.setCodec(QTextCodec::codecForName("cp1251"));

    QMap<int, int> job;
    job.insert(1, 3); // Опер. весов
    job.insert(2, 4); // Опер. линии
    job.insert(3, 5); // RACK
    job.insert(4, 6); // 4600

    QMap<int, int> attest;
    attest.insert(1,12);//Пригорнев А.А
    attest.insert(2,7); //Волков
    attest.insert(3,9); //Баштовой А.В.


    QSqlDatabase::database().transaction();
//    int i = 0;
    while (!stream.atEnd()) {
        QString s = stream.readLine();
        QStringList list = s.split(';');

//        i++;
//        if (i == 5) {
//            return;
//        }
        //OPER_NAME;SURNAME;PATRNAME;SM_ID;JOB_ID;TEMP_OPER;DATE_ATTEST;DATE_BEGIN_EDUCAT;DATE_REMUVE_ATTEST;ATTESTATOR_ID

        sql.bindValue(":name", list.at(1).simplified() + " " + list.at(0).simplified() + " " + list.at(2).simplified());
        sql.bindValue(":date_begin_education",dateFromString(list.at(7)));
        sql.bindValue(":date_attestation",dateFromString(list.at(6)));
        sql.bindValue(":remove_attestation",dateFromString(list.at(8)));
        sql.bindValue(":attestator",attest.value(list.at(9).toInt()));
        sql.bindValue(":job",job.value(list.at(4).toInt()));
        sql.bindValue(":shift",list.at(3).toInt());
        sql.bindValue(":down",0);


        if (!sql.exec()) {
            qWarning() << sql.lastError();
            QSqlDatabase::database().rollback();
            return;
        }
    }

    QSqlDatabase::database().commit();
}

void OperatorTableWidget::filter()
{
    if (ui->tabs->currentIndex() == 0) {
        m_model->setOperatorState(OperatorModel::AllState);
    } else if (ui->tabs->currentIndex() == 1) {
        m_model->setOperatorState(OperatorModel::EducationgState);
    } else if (ui->tabs->currentIndex() == 2) {
        m_model->setOperatorState(OperatorModel::AttestatedState);
    } else if (ui->tabs->currentIndex() == 3) {
        m_model->setOperatorState(OperatorModel::RemovedState);
    }

    m_model->setLocation(currentLocation());

    m_model->populate();
}

QString OperatorTableWidget::selectedIds() const
{
    QString str;
    QListIterator<QModelIndex> idx(ui->tableView->selectionModel()->selectedRows());
    while (idx.hasNext()) {
        if (!str.isEmpty()) {
            str.append(",");
        }

        str.append(m_proxyModel->index(idx.next().row(), OperatorModel::IdCol).data().toString());
    }

    return str;
}

QDate OperatorTableWidget::dateFromString(const QString &string)
{
    //16.9.2010 00:00:00
    QRegExp exp("^(\\d{1,2})[-. \\/,](\\d{1,2})[-. \\/,](\\d{2,4})");

    if (exp.indexIn(string) != -1) {
        QDate date;
        date.setDate(exp.cap(3).toInt(), exp.cap(2).toInt(), exp.cap(1).toInt());
        return date;
    } else {
        return QDate();
    }


}
