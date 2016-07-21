#ifndef HUE_GLOBAL_H
#define HUE_GLOBAL_H

#ifndef Q_DECL_EXPORT
#  ifdef Q_OS_WIN
#    define Q_DECL_EXPORT __declspec(dllexport)
#  elif defined(QT_VISIBILITY_AVAILABLE)
#    define Q_DECL_EXPORT __attribute__((visibility("default")))
#  endif
#  ifndef Q_DECL_EXPORT
#    define Q_DECL_EXPORT
#  endif
#endif
#ifndef Q_DECL_IMPORT
#  if defined(Q_OS_WIN)
#    define Q_DECL_IMPORT __declspec(dllimport)
#  else
#    define Q_DECL_IMPORT
#  endif
#endif

#if defined(PM_EXPORT)
#  define PM_WIDGET_EXPORT Q_DECL_EXPORT
#else
#  define PM_WIDGET_EXPORT Q_DECL_EXPORT
#endif

#endif // HUE_GLOBAL_H
