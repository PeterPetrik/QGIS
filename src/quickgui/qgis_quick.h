#ifndef QGIS_QUICK_H
#define QGIS_QUICK_H

#ifndef QUICK_EXPORT
#  ifdef qgis_quick_EXPORTS
      /* We are building this library */
#     define QUICK_EXPORT __attribute__((visibility("default")))
#  else
      /* We are using this library */
#     define QUICK_EXPORT __attribute__((visibility("default")))
#  endif
#endif //QUICK_EXPORT

#ifndef QUICK_NO_EXPORT
#  define QUICK_NO_EXPORT __attribute__((visibility("hidden")))
#endif

#endif // QGIS_QUICK_H
