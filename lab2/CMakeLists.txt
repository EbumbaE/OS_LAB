cmake_minimum_required(VERSION 3.22.0)

project(OS_LAB LANGUAGES C)

set(MAIN_HEADERS include/main.h include/include.h)
set(MAIN_SOURCE src/main.c)

add_executable(OS_LAB ${MAIN_SOURCE} ${MAIN_HEADERS})

project(CHILD1 LANGUAGES C)
project(CHILD2 LANGUAGES C)

set(CHILD_HEADERS include/workWithFile.h include/include.h)
set(CHILD_SOURCE src/workWithFile.c)

add_executable(CHILD1 ${CHILD_HEADERS} ${CHILD_SOURCE} src/child1.c)
add_executable(CHILD2 ${CHILD_HEADERS} ${CHILD_SOURCE} src/child2.c)