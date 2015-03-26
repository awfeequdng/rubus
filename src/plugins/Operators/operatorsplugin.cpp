#include "operatorsplugin.h"
#include "operatorsconstants.h"
#include "widgets/operatortablewidget.h"

#include <QtPlugin>
#include <QDebug>

using namespace Core;

static OperatorsPlugin *m_instance = 0;

OperatorsPlugin::OperatorsPlugin(QObject *parent) :
    IPlugin(parent)
{
    m_instance = this;
}

OperatorsPlugin::~OperatorsPlugin()
{
}


QString OperatorsPlugin::name() const
{
    return "Operators";
}

int OperatorsPlugin::version() const
{
    return 1;
}

bool OperatorsPlugin::initialize()
{
    m_operatorTableWdg = new OperatorTableWidget(0);
    ICore::registerWidget("Operators", m_operatorTableWdg);

    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Operators, OperatorsPlugin)
#else
#endif

