cmake_minimum_required(VERSION 3.22.0)
project(OS_LAB LANGUAGES C)

add_definitions("-DCMAKE_TOOLCHAIN_FILE=C:/Users/Ivanh/vcpkg/scripts/buildsystems/vcpkg.cmake")

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


set(INCLUDE include/include.h)

set(A_SOURCE src/a.c)
add_executable(A ${A_SOURCE} ${INCLUDE})
target_link_libraries(A PRIVATE lib_msgq libzmq libzmq-static)

set(B_SOURCE src/b.c)
add_executable(B ${B_SOURCE} ${INCLUDE})
target_link_libraries(B PRIVATE lib_msgq libzmq libzmq-static)

set(C_SOURCE src/c.c)
add_executable(C ${C_SOURCE} ${INCLUDE})
target_link_libraries(C PRIVATE lib_msgq libzmq libzmq-static)
