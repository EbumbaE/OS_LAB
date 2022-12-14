cmake_minimum_required(VERSION 3.22.0)
project(OS_LAB LANGUAGES C)

add_definitions("-DCMAKE_TOOLCHAIN_FILE=C:/Users/Ivanh/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(TREE_SOURCE src/tree.c)
set(TREE_INCLUDE include/tree.h)
add_library(
        lib_tree STATIC
        ${TREE_SOURCE}
        ${TREE_INCLUDE}
)

set(MSGQ_SOURCE src/msg_q.c)
set(MSGQ_INCLUDE include/msg_q.h)
add_library(
        lib_msgq STATIC
        ${MSGQ_SOURCE}
        ${MSGQ_INCLUDE}
)

INCLUDE_DIRECTORIES(
        C:/Users/Ivanh/vcpkg/installed/x86-windows/include/
)
find_package(ZeroMQ CONFIG REQUIRED)


set(MAIN_HEADERS include/include.h)
set(MAIN_SOURCE src/main.c)
add_executable(OS_LAB ${MAIN_SOURCE} ${MAIN_HEADERS})
target_link_libraries(OS_LAB PRIVATE lib_msgq libzmq libzmq-static)


set(CHILD_HEADERS include/child.h)
set(CHILD_SOURCE src/child.c)
add_executable(CHILD ${CHILD_SOURCE} ${CHILD_HEADERS})
target_link_libraries(CHILD PRIVATE lib_msgq libzmq libzmq-static)

set(PARENT_HEADERS include/parent.h)
set(PARENT_SOURCE src/parent.c)
add_executable(PARENT ${PARENT_SOURCE} ${PARENT_HEADERS})
target_link_libraries(PARENT PRIVATE lib_msgq libzmq libzmq-static)

set(TEST_TREE_HEADERS include/test_tree.h)
set(TEST_TREE_SOURCE src/test_tree.c)
add_executable(test_tree ${TEST_TREE_SOURCE} ${TEST_TREE_HEADERS})
target_link_libraries(test_tree PRIVATE lib_tree)

set(ORCHESTRA_SOURCE src/orchestra.c)
set(ORCHESTRA_INCLUDE include/orchestra.h include/tree.h)
add_executable(ORCHESTRA ${ORCHESTRA_SOURCE} ${ORCHESTRA_HEADERS})
target_link_libraries(ORCHESTRA PRIVATE lib_msgq lib_tree libzmq libzmq-static)