include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

add_library(CPUID STATIC ${SRC_LIST})
