cmake_minimum_required(VERSION 3.10)

set(project_name bsp)

# project(${project_name})
add_library(${project_name} INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

target_compile_definitions(${project_name} INTERFACE 

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

target_sources(${project_name} INTERFACE
    ${SOURCES}
)

target_include_directories(${project_name} INTERFACE
    ${INC_DIRS}
)

target_link_directories(${project_name} INTERFACE
)

target_link_libraries(${project_name} INTERFACE
)

# Validate that ${project_name} code is compatible with C standard
# if(CMAKE_C_STANDARD LESS 11)
#     message(ERROR "Generated code requires C11 or higher")
# endif()


