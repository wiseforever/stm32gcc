#include "sdkconfig.h"


//1,支持os
#define SYSTEM_SUPPORT_OS     0

#if (SYSTEM_SUPPORT_OS == 0)

#if __has_include("app_main.h")
#include "app_main.h"
#endif

#elif (SYSTEM_SUPPORT_OS == 1)

    #include "FreeRTOS.h"
    
#endif
