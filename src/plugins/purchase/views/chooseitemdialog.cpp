#include "chooseitemdialog.h"
#include "ui_chooseitemdialog.h"
#include "models/equipmenttypemodel.h"

#include "../../plugins/base/baseplugin.h"

#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QtSql>
#include <QMessageBox>

ChooseItemDialog::ChooseItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseItemDialog)
{
    ui->setupUi(this);

    m_eqTypeModel = new EquipmentTypeModel(this);
    m_itemModel = new QSqlQueryModel(this);
    m_itemProxyModel = new QSortFilterProxyModel(this);
    m_itemProxyModel->setFilterKeyColumn(1);
    m_itemProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_itemProxyModel->setSourceModel(m_itemModel);
    ui->tableView->setModel(m_itemProxyModel);


    connect(ui->ckEqType, SIGNAL(stateChanged(int)), SLOT(appendFilter()));
    connect(ui->cmbEqType, SIGNAL(currentIndexChanged(int)), SLOT(appendFilter()));
    connect(ui->edFind, SIGNAL(textChanged(QString)), m_itemProxyModel, SLOT(setFilterWildcard(QString)));
}

ChooseItemDialog::~ChooseItemDialog()
{
    delete ui;
}

void ChooseItemDialog::populate()
{
    m_eqTypeModel->populate();
    ui->cmbEqType->setModel(m_eqTypeModel);

    if (m_eqId > -1) {
        QModelIndex index = m_eqTypeModel->indexById(m_eqId);
        if (index.isValid()) {
            ui->cmbEqType->setRootModelIndex(index.parent());
            ui->cmbEqType->setCurrentIndex(index.row());
        }
    }
}

int ChooseItemDialog::exec(int itemId)
{
    qDebug() << BasePlugin::instance();
    m_itemId = itemId;

    populate();
    appendFilter();

    return QDialog::exec();
}

int ChooseItemDialog::selectedItemId() const
{
    return ui->tableView->currentId().toInt();
}

int ChooseItemDialog::selectedEquipmentType() const
{
    return m_eqTypeModel->index(ui->cmbEqType->currentIndex(), 0, ui->cmbEqType->rootModelIndex()).data(Qt::EditRole).toInt();
}

void ChooseItemDialog::appendFilter()
{
    QString where;
    if (ui->ckEqType->isChecked()) {
        where.append(QString("WHERE it_id IN (SELECT ie_item FROM items_equipment_types WHERE ie_equipment_type = %1)")
                     .arg(m_eqTypeModel->index(ui->cmbEqType->currentIndex(), 0, ui->cmbEqType->rootModelIndex()).data(Qt::EditRole).toInt()));
    }

    m_itemModel->setQuery(QString("SELECT it_id, it_name, it_article, un_name FROM items "
                          "JOIN units ON un_id = it_unit %1")
                          .arg(where));

    if (m_itemModel->lastError().isValid()) {
        qDebug() << m_itemModel->lastError();
        QMessageBox::critical(this, "Error", m_itemModel->lastError().text());
    }

    m_itemModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_itemModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_itemModel->setHeaderData(2, Qt::Horizontal, tr("Article"));
    m_itemModel->setHeaderData(3, Qt::Horizontal, tr("Unit"));
    ui->tableView->horizontalHeader()->setDefaultSectionSize(0, 50);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(2, 120);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(3, 50);
}
