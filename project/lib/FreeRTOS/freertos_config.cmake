# freertos_config.cmake

# User-defined settings
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "FreeRTOS port name")
set(FREERTOS_HEAP 4 CACHE STRING "FreeRTOS heap")

# 定义 freertos_config INTERFACE 库
add_library(freertos_config INTERFACE)
# 指定 FreeRTOSConfig.h 所在目录 若是在FreeRTOS所在的include目录下，则不需要指定
# target_include_directories(freertos_config SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)  # 添加需要的编译定义
target_compile_options(freertos_config INTERFACE -Wno-unused-variable)