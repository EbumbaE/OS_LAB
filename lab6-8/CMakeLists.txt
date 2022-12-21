cmake_minimum_required(VERSION 3.22.0)
project(OS_LAB LANGUAGES C)

set(TREE_SOURCE src/tree.c)
set(TREE_INCLUDE include/tree.h)
add_library(
        lib_tree STATIC
        ${TREE_SOURCE}
        ${TREE_INCLUDE}
)

set(MAIN_HEADERS include/main.h)
set(MAIN_SOURCE src/main.c)
add_executable(OS_LAB ${MAIN_SOURCE} ${MAIN_HEADERS})

target_link_libraries(OS_LAB PRIVATE lib_tree)