cmake_minimum_required(VERSION 3.10)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
############### wise start ###############
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs) # 静态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 动态库输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 可执行文件输出目录

set(CMAKE_BUILD_TYPE "Release") # Debug Release
############### wise end. ###############

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME project)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for C 、C++ and ASM languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

############### wise start ###############
# set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/sdram.ld\"")
# list(APPEND CMAKE_C_LINK_FLAGS 
#     -T${CMAKE_SOURCE_DIR}/sdram.ld
# )


add_subdirectory(app)
add_subdirectory(bsp)
add_subdirectory(lib)
############### wise end. ###############
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
    app
    bsp
    log
    freertos_kernel
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE 
    -lrdimon
    -u_printf_float
)

############### wise start ###############
# Set compiler options
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall           # 启用所有警告
    -Wextra         # 启用额外的警告
    -Wpedantic      # 启用兼容性警告 要求严格遵循标准
    -Wno-unused-variable    # 禁用未使用的变量警告
    -Wno-unused-parameter   # 禁用未使用的参数警告
    -Wno-unused-function    # 禁用未使用的函数警告
    -Wno-unused-but-set-variable   # 禁用未使用但被设置的变量警告
    # -Wno-format  # 禁用格式化字符串警告
    -Wno-comment   # 禁用多行注释警告
    -Wno-pedantic  # 禁用不严谨的警告 如: 空文件等
)

# # 添加生成 hex 文件的命令
# add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
#     POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} -O ihex bin/${CMAKE_PROJECT_NAME}.elf bin/${CMAKE_PROJECT_NAME}.hex
#     DEPENDS bin/${CMAKE_PROJECT_NAME}.elf
#     COMMENT "Generating HEX file"
# )

# # 添加生成 bin 文件的命令
# add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
#     POST_BUILD
#     COMMAND ${CMAKE_OBJCOPY} -O binary bin/${CMAKE_PROJECT_NAME}.elf bin/${CMAKE_PROJECT_NAME}.bin
#     DEPENDS bin/${CMAKE_PROJECT_NAME}.elf
#     COMMENT "Generating BIN file"
# )

# 添加生成 hex 和 bin 文件的命令
add_custom_command(
    OUTPUT bin/${CMAKE_PROJECT_NAME}.hex bin/${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> bin/${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> bin/${CMAKE_PROJECT_NAME}.bin
    DEPENDS $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    # COMMENT "Generating HEX and BIN files"
)
# message(STATUS "Target file: $<TARGET_FILE:${CMAKE_PROJECT_NAME}>")


# 创建一个生成目标，方便用户单独生成 hex 和 bin 文件
add_custom_target(build-hex ALL DEPENDS bin/${CMAKE_PROJECT_NAME}.hex)
add_custom_target(build-bin ALL DEPENDS bin/${CMAKE_PROJECT_NAME}.bin)
############### wise end. ###############
