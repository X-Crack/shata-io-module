#ifndef __EVENT_PLATFORM_H__
#define __EVENT_PLATFORM_H__
#include <event_arch.h>
#include <event_stdint.h>

#ifdef H_OS_WINDOWS
#include <Windows.h>
#endif

#ifdef H_OS_WINDOWS
#   define FORCEINLINE __forceinline
#   define NOFORCEINLINE __declspec (noinline)
#else
#   define FORCEINLINE __attribute__ ((always_inline))
#   define NOFORCEINLINE __attribute__ ((noinline))
#endif

#ifdef H_OS_WINDOWS
#   if defined(_WIN32)
#       define H_OS_X86
#   elif defined(_WIN64)
#       define H_OS_X64
#   endif
#endif

#if (defined(_DEBUG) || defined(DEBUG) || defined(_DEBUG_))
#   define EVENT_DEBUG_MODE
#endif

#ifdef H_OS_WINDOWS
#   define EVPP_THREAD_YIELD SwitchToThread
#else
#   define EVPP_THREAD_YIELD std::this_thread::yield
#endif

#define USE_PARAMETER(P) (P)

#ifdef H_OS_WINDOWS
#   pragma warning( disable: 4251 )
#   pragma warning( disable: 4996 )
#   pragma warning( disable: 4244 4251 4355 4715 4800 4996 4005 4819)
#   pragma warning( disable: 4505 )
#   pragma warning( disable: 4530 )
#   pragma warning( disable: 4577 )
#   pragma warning( disable: 4503 )
#   pragma warning( disable: 4458 )
#   pragma warning( disable: 4100 )
#   pragma warning( disable: 4702 )
#endif

#endif // __EVENT_PLATFORM_H__