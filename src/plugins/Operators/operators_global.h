#ifndef OPERATORS_GLOBAL_H
#define OPERATORS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OPERATORS_LIBRARY)
#  define OPERATORS_EXPORT Q_DECL_EXPORT
#else
#  define OPERATORS_EXPORT Q_DECL_IMPORT
#endif

#endif // OPERATORS_GLOBAL_H

