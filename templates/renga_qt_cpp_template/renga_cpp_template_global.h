#ifndef RENGA_CPP_TEMPLATE_GLOBAL_H
#define RENGA_CPP_TEMPLATE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RENGA_CPP_TEMPLATE_LIBRARY)
#  define RENGA_CPP_TEMPLATE_EXPORT Q_DECL_EXPORT
#else
#  define RENGA_CPP_TEMPLATE_EXPORT Q_DECL_IMPORT
#endif

#endif // RENGA_CPP_TEMPLATE_GLOBAL_H
