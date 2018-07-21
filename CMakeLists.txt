cmake_minimum_required(VERSION 3.1)

project(CPUID)

set(C_STANDARD 11)
set(CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-R,'$$ORIGIN',--enable-new-dtags")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(libs)
add_subdirectory(tests)

add_custom_target(MainTest ALL DEPENDS tests)
add_custom_command(TARGET MainTest POST_BUILD COMMAND tests)