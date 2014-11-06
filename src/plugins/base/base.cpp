#include "base.h"
#include "constants.h"
#include "widgets/standardtabledialog.h"

#include <QtPlugin>
#include <QAction>
#include <QDebug>

using namespace Core;

static Base *m_instance = 0;

Base::Base(QObject *parent) :
    IPlugin(parent)
{
    m_instance = this;
}


QString Base::name() const
{
    return "Base";
}

int Base::version() const
{
    return 1;
}

bool Base::initialize()
{
    m_acUserManager = new QAction(tr("Users"),this);
    connect(m_acUserManager, SIGNAL(triggered()), SLOT(showUserManager()));
    ICore::registerAction(Constants::A_USERMANAGER, m_acUserManager);

    m_acReportManager = new QAction(tr("Report manager"),this);
    connect(m_acReportManager, SIGNAL(triggered()), SLOT(showReportManager()));
    ICore::registerAction(Constants::A_REPORTMANAGER, m_acReportManager);

    m_acLocations = new QAction(tr("Locations"),this);
    connect(m_acLocations, SIGNAL(triggered()), SLOT(showLocationTable()));
    ICore::registerAction(Constants::A_LOCATIONS, m_acLocations);

    m_acContractors = new QAction(tr("Contractors"),this);
    connect(m_acContractors, SIGNAL(triggered()), SLOT(showContractorTable()));
    ICore::registerAction(Constants::A_CONTRACTORS, m_acContractors);

    m_acItems = new QAction(tr("Items"),this);
    connect(m_acItems, SIGNAL(triggered()), SLOT(showItemTable()));
    ICore::registerAction(Constants::A_ITEMS, m_acItems);

    m_itemDialog = new StandardTableDialog();
    m_itemDialog->setWindowTitle(tr("Items"));
    m_itemDialog->setReportMenu(Constants::A_ITEMS);

    return true;
}

void Base::showUserManager()
{
    qDebug() << "users";
}

void Base::showReportManager()
{
    qDebug() << "reprots";
}

void Base::showLocationTable()
{

}

void Base::showContractorTable()
{

}

void Base::showItemTable()
{
    m_instance->m_itemDialog->show();
    m_instance->m_itemDialog->activateWindow();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Base, Base)
#else
#endif
