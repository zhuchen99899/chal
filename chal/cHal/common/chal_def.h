#ifndef __CHAL_DEF_
#define __CHAL_DEF_
#include "stdint.h"
#include "chal_config.h"
/* public typedef ----------------------------------------------------------- */
typedef enum CHal_Err
{
    CHAL_OK                             = 0,
    CHAL_ERROR                          = -1,
    CHAL_ERR_EMPTY                      = -2,
    CHAL_ERR_FULL                       = -3,
    CHAL_ERR_TIMEOUT                    = -4,
    CHAL_ERR_BUSY                       = -5,
    CHAL_ERR_NO_MEMORY                  = -6,
    CHAL_ERR_IO                         = -7,
    CHAL_ERR_INVALID                    = -8,
    CHAL_ERR_MEM_OVERLAY                = -9,
    CHAL_ERR_MALLOC                     = -10,
    CHAL_ERR_NOT_ENOUGH                 = -11,
    CHAL_ERR_NO_SYSTEM                  = -12,
    CHAL_ERR_BUS                        = -13,
} CHal_Err_t;


/* mutex define ------------------------------------------------------------ */

#if CHAL_OS_EN  //use os mutex

    #define __CHAL_MUTEX_NEW(__HANDLE__)                                                   \
    do{                                                                                    \
                        (__HANDLE__)->mutex = osMutexNew((__HANDLE__)->mutex_attr);      \
                                    }while (0U)

    #define __CHAL_MUTEX_DELETE(__HANDLE__)                                                 \
    do{                                                                                    \
                    osStatus_t ret     = osMutexDelete((__HANDLE__)->mutex);              \
                    if (ret==osOK)                                                        \
                    {                                                                     \
                    (__HANDLE__)->mutex=NULL;                                             \
                    }                                                                     \
                    /*assert(ret)*/                                                       \
                                    }while (0U)

    #define __CHAL_MUTEX_ACQUIRE(__HANDLE__)                                                              \
                                    do{                                                                   \
                                    osStatus_t ret= osMutexAcquire((__HANDLE__)->mutex, osWaitForever);   \
                                    /*assert(ret)*/                                                       \
                                    }while (0U)

    #define __CHAL_MUTEX_RELEASE(__HANDLE__)                                    \
                                    do{                                         \
                    osStatus_t ret     = osMuteRelease((__HANDLE__)->mutex);    \
                    /*assert(ret)*/                                             \
                                    }while (0U)

#else  //no_os mutex (lock)
typedef enum CHal_Lock
{
CHAL_LOCK_NULL=0,
CHAL_LOCK,
CHAL_UNLOCK
}Chal_Lock_t;


    #define __CHAL_MUTEX_NEW(__HANDLE__)                                       \
    do{                                                                        \
                        (__HANDLE__)->lock = CHAL_UNLOCK;                     \
                                    }while (0U)

    #define __CHAL_MUTEX_DELETE(__HANDLE__)                                    \
                                    do{                                        \
                                        (__HANDLE__)->lock = CHAL_LOCK_NULL;   \
                                    }while (0U)

    #define __CHAL_MUTEX_ACQUIRE(__HANDLE__)                                   \
                                    do{                                        \
                                        if((__HANDLE__)->lock == CHAL_LOCK)    \
                                        {                                      \
                                        return CHAL_ERR_BUSY;               \
                                        }                                      \
                                        else                                   \
                                        {                                      \
                                        (__HANDLE__)->lock = CHAL_LOCK;     \
                                        }                                      \
                                    }while (0U)

    #define __CHAL_MUTEX_RELEASE(__HANDLE__)                                  \
                                    do{                                       \
                                        (__HANDLE__)->lock = CHAL_UNLOCK;     \
                                    }while (0U)
                                    
#endif



/* public define ------------------------------------------------------------ */
#define EXPORT_ID_INIT                  (0xa5a5a5a5)


/* Compiler Related Definitions */
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARM Compiler */

    #include <stdarg.h>
    #define CHAL_SECTION(x)             __attribute__((section(x)))
    #define CHAL_USED                   __attribute__((used))
    #define CHAL_ALIGN(n)               __attribute__((aligned(n)))
    #define CHAL_WEAK                   __attribute__((weak))
    #define chal_inline                 static __inline

#elif defined (__IAR_SYSTEMS_ICC__)           /* for IAR Compiler */

    #include <stdarg.h>
    #define CHAL_SECTION(x)             @ x
    #define CHAL_USED                   __root
    #define CHAL_PRAGMA(x)              _Pragma(#x)
    #define CHAL_ALIGN(n)               CHAL_PRAGMA(data_alignment=n)
    #define CHAL_WEAK                   __weak
    #define chal_inline                 static inline

#elif defined (__GNUC__)                      /* GNU GCC Compiler */

    #include <stdarg.h>
    #define CHAL_SECTION(x)             __attribute__((section(x)))
    #define CHAL_USED                   __attribute__((used))
    #define CHAL_ALIGN(n)               __attribute__((aligned(n)))
    #define CHAL_WEAK                   __attribute__((weak))
    #define chal_inline                 static inline

#else
    #error The current compiler is NOT supported!
#endif

#if defined(__x86_64__) || defined(__aarch64__)
typedef int64_t                         chal_pointer_t;
#elif defined(__i386__) || defined(__arm__)
typedef int32_t                         chal_pointer_t;
#else
    #error The currnet CPU is NOT supported!
#endif


#endif // !1

