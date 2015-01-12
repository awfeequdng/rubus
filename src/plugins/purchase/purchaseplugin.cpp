#include "purchaseplugin.h"
#include "purchaseconstants.h"
#include "views/requesttablewidget.h"

#include <QtPlugin>
#include <QDebug>
#include <QApplication>

using namespace Core;

static PurchasePlugin *m_instance = 0;

PurchasePlugin::PurchasePlugin(QObject *parent) :
    IPlugin(parent)
{
    m_instance = this;

}

PurchasePlugin::~PurchasePlugin()
{
}


QString PurchasePlugin::name() const
{
    return "Purchase";
}

int PurchasePlugin::version() const
{
    return 1;
}

bool PurchasePlugin::initialize()
{
    m_requestTableWidget = new RequestTableWidget();
    ICore::registerWidget(Constants::W_PURCHASE_REQUESTS, m_requestTableWidget);

    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(purchase, PurchasePlugin)
#else
#endif

