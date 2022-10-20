cmake_minimum_required(VERSION 3.22.0)

project(OS_LAB LANGUAGES C)

set(MAIN_HEADERS include/main.h include/include.h)
set(MAIN_SOURCE src/main.c)

add_executable(OS_LAB ${MAIN_SOURCE} ${MAIN_HEADERS})