cmake_minimum_required(VERSION 3.22.0)
project(OS_LAB LANGUAGES C)

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

set(MAIN_HEADERS include/include.h)
set(MAIN_SOURCE src/main.c)
add_executable(os_lab ${MAIN_SOURCE} ${MAIN_HEADERS})
target_link_libraries(os_lab PRIVATE lib_msgq libzmq.so)


set(CHILD_HEADERS include/child.h)
set(CHILD_SOURCE src/child.c)
add_executable(child ${CHILD_SOURCE} ${CHILD_HEADERS})
target_link_libraries(child PRIVATE lib_msgq libzmq.so)

set(PARENT_HEADERS include/parent.h)
set(PARENT_SOURCE src/parent.c)
add_executable(parent ${PARENT_SOURCE} ${PARENT_HEADERS})
target_link_libraries(parent PRIVATE lib_msgq libzmq.so)

set(TEST_TREE_HEADERS include/test_tree.h)
set(TEST_TREE_SOURCE src/test_tree.c)
add_executable(test_tree ${TEST_TREE_SOURCE} ${TEST_TREE_HEADERS})
target_link_libraries(test_tree PRIVATE lib_tree)

set(ORCHESTRA_SOURCE src/orchestra.c)
set(ORCHESTRA_INCLUDE include/orchestra.h include/tree.h)
add_executable(orchestra ${ORCHESTRA_SOURCE} ${ORCHESTRA_HEADERS})
target_link_libraries(orchestra PRIVATE lib_msgq lib_tree libzmq.so)