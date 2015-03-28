#include "operatortablewidget.h"
#include "ui_operatortablewidget.h"
#include "operatoreditwidget.h"
#include "models/operatormodel.h"
#include "widgets/editdialog.h"

#include <QSortFilterProxyModel>
#include <QSettings>

OperatorTableWidget::OperatorTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperatorTableWidget)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/operator.png"));

    m_editWidget = new OperatorEditWidget();
    m_editDlg = new EditDialog(m_editWidget, this);

    m_model = new OperatorModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

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

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(editCurrent()));

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
    populate();

    QWidget::showEvent(e);
}

int OperatorTableWidget::currentId() const
{
    return ui->tableView->currentId().toInt();
}

void OperatorTableWidget::add()
{
    if (m_editDlg->exec()) {
        populate();
        ui->tableView->setCurrentId(m_editWidget->id());
    }
}

void OperatorTableWidget::editCurrent()
{
    if (m_editDlg->exec(currentId())) {
        populate();
        ui->tableView->setCurrentId(m_editWidget->id());
    }
}

void OperatorTableWidget::removeSelected()
{

}

void OperatorTableWidget::populate()
{
    m_model->populate();
}
