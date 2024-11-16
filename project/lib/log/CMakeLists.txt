cmake_minimum_required(VERSION 3.10)

# Option to build shared libraries (as opposed to static), default: OFF
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(project_name log)

# project(${project_name})
# add_library(${project_name} 
# STATIC # 静态库
#     ""
# )

# if(MSVC)
#     set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#     set(BUILD_SHARED_LIBS TRUE)
# endif()

# add_library(${project_name} 
# SHARED # 动态库
#     ""
# )

if (BUILD_SHARED_LIBS)
  add_library(${project_name} SHARED)
else()
  add_library(${project_name} STATIC)
endif()

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

target_compile_definitions(${project_name} 
PRIVATE

)

### Get source file
set(COMPILE_DIR ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE SOURCES 
    ${COMPILE_DIR}/*.c 
    ${COMPILE_DIR}/*.cc 
    ${COMPILE_DIR}/*.cpp 
    ${COMPILE_DIR}/*.s
    ${COMPILE_DIR}/*.S
)

### Get header file
file(GLOB_RECURSE HEADER_FILES 
    ${COMPILE_DIR}/*.h
    ${COMPILE_DIR}/*.hpp
    ${COMPILE_DIR}/*.hxx
)
set(HEADER_DIRS)
foreach(header_file ${HEADER_FILES})
    get_filename_component(dir ${header_file} DIRECTORY)
    list(APPEND HEADER_DIRS ${dir})
endforeach()

### Remove duplicate directories
list(REMOVE_DUPLICATES HEADER_DIRS)
set(INC_DIRS ${HEADER_DIRS})

target_sources(${project_name} 
PRIVATE
    ${SOURCES}
)

target_include_directories(${project_name} 
PUBLIC 
    ${INC_DIRS}
)

# Validate that ${project_name} code is compatible with C standard
# if(CMAKE_C_STANDARD LESS 11)
#     message(ERROR "Generated code requires C11 or higher")
# endif()


