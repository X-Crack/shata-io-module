#ifndef __EVENT_EXPORT_H__
#define __EVENT_EXPORT_H__
#include <event_arch.h>

#ifndef TCP_SERVICE_STATIC
#   define TCP_SERVICE_STATIC
#endif

#ifdef __cplusplus
#   if __cplusplus > 201703L
#       define CPLUSPLUS_VERSION(x) x##" "##"C++ 2A"
#   elif __cplusplus == 201703L
#       define CPLUSPLUS_VERSION(x) x##" "##"C++ 17"
#   elif __cplusplus == 201402L
#       define CPLUSPLUS_VERSION(x) x##" "##"C++ 14"
#   elif __cplusplus == 201103L
#       define CPLUSPLUS_VERSION(x) x##" "##"C++ 11"
#   elif __cplusplus == 199711L
#       define CPLUSPLUS_VERSION(x) x##" "##"C++ 98"
#else
#       define CPLUSPLUS_VERSION(x) x##" "##"C++ Unknown"
#   endif
#endif

#ifdef H_OS_WINDOWS
#   if !defined(DEV_OUTPUT_LOG)
#       define DEV_OUTPUT_LOG            CPLUSPLUS_VERSION
#   endif
#   if !defined(_DLL) && !defined(_DEBUG)
#       ifndef TCP_SERVICE_STATIC
#           ifndef TCP_SERVICE_EXPORT_STATIC
#               define TCP_SERVICE_EXPORT_STATIC
#           endif
#       endif
#       pragma message(DEV_OUTPUT_LOG("Compilation mode MT"))
#   elif !defined(_DLL) && defined(_DEBUG)
#       ifndef TCP_SERVICE_STATIC
#           ifndef TCP_SERVICE_EXPORT_STATIC
#               define TCP_SERVICE_EXPORT_STATIC
#           endif
#       endif
#       pragma message(DEV_OUTPUT_LOG("Compilation mode MTd"))
#   elif defined(_DLL) && !defined(_DEBUG)
#       ifndef TCP_SERVICE_STATIC
#           ifdef TCP_SERVICE_EXPORT_STATIC
#               undef TCP_SERVICE_EXPORT_STATIC
#           endif
#       endif
#       pragma message(DEV_OUTPUT_LOG("Compilation mode MD"))
#   elif defined(_DLL) && defined(_DEBUG)
#       ifndef TCP_SERVICE_STATIC
#           ifdef TCP_SERVICE_EXPORT_STATIC
#               undef TCP_SERVICE_EXPORT_STATIC
#           endif
#       endif
#       pragma message(DEV_OUTPUT_LOG("Compilation mode MDd"))
#   endif
#endif

#if defined(H_OS_WINDOWS)
#       define TCP_SERVICE_DECL_EXPORT __declspec(dllexport)
#       define TCP_SERVICE_DECL_IMPORT __declspec(dllimport)
#else
#       define TCP_SERVICE_DECL_EXPORT
#       define TCP_SERVICE_DECL_IMPORT
#endif

#ifndef TCP_SERVICE_STATIC
#   if defined(TCP_SERVICE_EXPORT_STATIC)
#       define TCP_SERVICE_EXPORT TCP_SERVICE_DECL_EXPORT
#   else
#       define TCP_SERVICE_EXPORT TCP_SERVICE_DECL_IMPORT
#   endif
#else
#       define TCP_SERVICE_EXPORT
#endif

#endif  // __EVENT_EXPORT_H__