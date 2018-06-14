cmake_minimum_required(VERSION 2.8.0)

include(CMakeForceCompiler)

option(USE_CLANG "build use clang++" OFF)

if(USE_CLANG)
	set(CMAKE_C_COMPILER clang CACHE INTERNAL "" FORCE)
	set(CMAKE_CXX_COMPILER clang++ CACHE INTERNAL "" FORCE)
else()
	set(CMAKE_C_COMPILER gcc-8 CACHE INTERNAL "" FORCE)
	set(CMAKE_CXX_COMPILER g++-8 CACHE INTERNAL "" FORCE)
endif()

set(CMAKE_C_FLAGS "-fPIC -Wall -march=native -std=gnu99" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_FLAGS "-fPIC -Wall -march=native -std=gnu++17 -fcheck-new -fexceptions" CACHE INTERNAL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-march=native -fPIC -Wl,-R,'$$ORIGIN',--enable-new-dtags -Wl,--gc-sections -Wl,--hash-style=gnu" CACHE INTERNAL "" FORCE)

project(CPUID)
set(TARGET_NAME CPUID)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

message(STATUS "CMAKE_C_FLAGS:" ${CMAKE_C_FLAGS})
message(STATUS "CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_EXE_LINKER_FLAGS:" ${CMAKE_EXE_LINKER_FLAGS})

include_directories(
    include
)

add_subdirectory(libs)
add_subdirectory(tests)

add_custom_target(MainTest ALL DEPENDS tests)
add_custom_command(TARGET MainTest POST_BUILD COMMAND tests)