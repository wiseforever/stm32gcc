# library project CMakeLists.txt file

add_subdirectory(log)

include(${CMAKE_CURRENT_LIST_DIR}/FreeRTOS/freertos_config.cmake)
add_subdirectory(FreeRTOS)


