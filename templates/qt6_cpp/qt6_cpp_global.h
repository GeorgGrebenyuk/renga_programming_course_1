#ifndef QT6_CPP_GLOBAL_H
#define QT6_CPP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT6_CPP_LIBRARY)
#  define QT6_CPP_EXPORT Q_DECL_EXPORT
#else
#  define QT6_CPP_EXPORT Q_DECL_IMPORT
#endif

#endif // QT6_CPP_GLOBAL_H
