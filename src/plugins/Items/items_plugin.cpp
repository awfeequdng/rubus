#include "items_plugin.h"
#include "rubusitem.h"

#include <qqml.h>

void ItemsPlugin::registerTypes(const char *uri)
{
    // @uri Items
    qmlRegisterType<RubusItem>(uri, 1, 0, "RubusItem");
}


