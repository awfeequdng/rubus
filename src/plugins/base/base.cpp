#include "base.h"

using namespace Core;

Base::Base(QObject *parent) :
    IPlugin(parent)
{
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

    return true;
}
