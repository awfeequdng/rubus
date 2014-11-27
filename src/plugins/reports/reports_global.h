#ifndef REPORTS_GLOBAL_H
#define REPORTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(REPORTS_LIBRARY)
#  define REPORTS_EXPORT Q_DECL_EXPORT
#else
#  define REPORTS_EXPORT Q_DECL_IMPORT
#endif

#endif // REPORTS_GLOBAL_H

