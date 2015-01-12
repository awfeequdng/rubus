#ifndef PURCHASE_GLOBAL_H
#define PURCHASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PURCHASE_LIBRARY)
#  define PURCHASE_EXPORT Q_DECL_EXPORT
#else
#  define PURCHASE_EXPORT Q_DECL_IMPORT
#endif

#endif // PURCHASE_GLOBAL_H

