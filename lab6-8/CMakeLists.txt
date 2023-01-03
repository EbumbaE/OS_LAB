cmake_minimum_required(VERSION 3.22.0)
project(OS_LAB LANGUAGES C)

set(TREE_SOURCE src/tree.c)
set(TREE_INCLUDE include/tree.h)
add_library(
        lib_tree STATIC
        ${TREE_SOURCE}
        ${TREE_INCLUDE}
)

set(ORCHESTRA_SOURCE src/orchestra.c)
set(ORCHESTRA_INCLUDE include/orchestra.h include/tree.h include/msg_q.h)
add_library(
        lib_orchestra STATIC
        ${ORCHESTRA_SOURCE}
        ${ORCHESTRA_INCLUDE}
)

set(MAIN_HEADERS include/include.h)
set(MAIN_SOURCE src/main.c)
add_executable(OS_LAB ${MAIN_SOURCE} ${MAIN_HEADERS}) 
target_link_libraries(OS_LAB PRIVATE lib_orchestra lib_tree)

INCLUDE_DIRECTORIES(
        C:/Users/Ivanh/vcpkg/installed/x86-windows/include/
)
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS C:/Users/Ivanh/vcpkg/installed/x86-windows/lib/
        )
target_link_libraries(OS_LAB PRIVATE ${ZeroMQ_LIBRARY})

set(CHILD_HEADERS include/child.h)
set(CHILD_SOURCE src/child.c)
add_executable(CHILD ${CHILD_SOURCE} ${CHILD_HEADERS})

set(TEST_TREE_HEADERS include/test_tree.h)
set(TEST_TREE_SOURCE src/test_tree.c)
add_executable(test_tree ${TEST_TREE_SOURCE} ${TEST_TREE_HEADERS})
target_link_libraries(test_tree PRIVATE lib_tree)
