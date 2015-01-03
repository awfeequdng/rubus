#include "contractors_plugin.h"
#include "contractor.h"

#include <qqml.h>

void ContractorsPlugin::registerTypes(const char *uri)
{
    // @uri Contractors
    qDebug() <<uri << "loaded";

    qmlRegisterType<Contractor>(uri, 1, 0, "Contractor");
}


