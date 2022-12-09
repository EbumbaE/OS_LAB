cmake_minimum_required(VERSION 3.22.0)
project(lab_5 LANGUAGES C)

set(DYNAMIC_SOURCE src/main_dy.c)
set(STATIC_SOURCE src/main_st.c)
set(MAIN_INCLUDE include/include.h)

set(LIBRARIES1_SOURCE src/lib1.c)
set(LIBRARIES2_SOURCE src/lib2.c)
set(LIBRARIES_INCLUDE include/lib.h)

add_library(
        lib1 SHARED
        ${LIBRARIES_INCLUDE}
        ${LIBRARIES1_SOURCE}
)

add_library(
        lib2 SHARED
        ${LIBRARIES_INCLUDE}
        ${LIBRARIES2_SOURCE}
)

add_library(
        lib_st1 STATIC
        ${LIBRARIES_INCLUDE}
        ${LIBRARIES2_SOURCE}
)

add_library(
        lib_st2 STATIC
        ${LIBRARIES_INCLUDE}
        ${LIBRARIES2_SOURCE}
)

if (UNIX)
        add_definitions(-DOS=UNIX)
endif()

if (APPLE)
        add_definitions(-DOS=APPLE)
endif()

if (WIN32)
        add_definitions(-DOS=WIN32)
endif()

add_executable(main_dy ${DYNAMIC_SOURCE})
add_executable(main_st1 ${STATIC_SOURCE})
add_executable(main_st2 ${STATIC_SOURCE})

target_link_libraries(main_st1 PRIVATE lib_st1)
target_link_libraries(main_st2 PRIVATE lib_st2)
