#ifndef __SDKCONFIG_H__
#define __SDKCONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif

#if (SYSTEM_SUPPORT_OS == 0)

    #if __has_include("app_main.h")
        #include "app_main.h"
    #endif

#elif (SYSTEM_SUPPORT_OS == 1)

    /* #include "FreeRTOS.h" */
    
#endif

#ifdef __cplusplus
}
#endif

#endif /* !__SDKCONFIG_H__ */


