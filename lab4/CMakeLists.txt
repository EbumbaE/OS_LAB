cmake_minimum_required(VERSION 3.22.0)

project(OS_LAB LANGUAGES C)

set(MAIN_HEADERS include/main.h include/include.h include/work_with_event.h)
set(MAIN_SOURCE src/main.c src/work_with_event.c)

add_executable(OS_LAB ${MAIN_SOURCE} ${MAIN_HEADERS})

project(CHILD1 LANGUAGES C)
project(CHILD2 LANGUAGES C)

set(CHILD_HEADERS include/work_with_file.h include/work_with_event.h include/include.h)
set(CHILD1_SOURCE src/work_with_file.c src/work_with_event.c src/child1.c)
set(CHILD2_SOURCE src/work_with_file.c src/work_with_event.c src/child2.c)

add_executable(CHILD1 ${CHILD_HEADERS} ${CHILD1_SOURCE})
add_executable(CHILD2 ${CHILD_HEADERS} ${CHILD2_SOURCE})