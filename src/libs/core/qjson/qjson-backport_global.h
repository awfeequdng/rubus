#ifndef QJSONBACKPORT_GLOBAL_H
#define QJSONBACKPORT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QJSONBACKPORT_LIBRARY)
#  define QJSONBACKPORTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QJSONBACKPORTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QJSONBACKPORT_GLOBAL_H