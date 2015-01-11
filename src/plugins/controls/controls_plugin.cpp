#include "controls_plugin.h"

#include <qqml.h>
#include <QDebug>

void ControlsPlugin::registerTypes(const char *uri)
{
    qDebug() << uri;
    //@uri Rubus.Widgets
    //qmlRegisterType<Widgets>(uri, 1, 0, "Widgets");
}


