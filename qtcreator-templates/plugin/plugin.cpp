#include "%PluginName:l%.%CppHeaderSuffix%"
#include "%ProjectName:l%constants.%CppHeaderSuffix%"

#include <QtPlugin>
#include <QDebug>

using namespace Core;

static %PluginName% *m_instance = 0;

%PluginName%::%PluginName%(QObject *parent) :
    IPlugin(parent)
{
    m_instance = this;
}

%PluginName%::~%PluginName%()
{
}


QString %PluginName%::name() const
{
    return "%ProjectName%";
}

int %PluginName%::version() const
{
    return %Version%;
}

bool %PluginName%::initialize()
{
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(%ProjectName%, %PluginName%)
#else
#endif
