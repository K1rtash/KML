#ifndef KML_ABI_H
#define KML_ABI_H

#if defined(_WIN32)
  #if defined(KML_SHARED_LIBS)
    #if defined(KML_BUILD_DLL)
      #define KML_API __declspec(dllexport)
    #else
      #define KML_API __declspec(dllimport)
    #endif
  #else
    #define KML_API
  #endif
#else
  #define KML_API
#endif

#endif // KML_ABI_H