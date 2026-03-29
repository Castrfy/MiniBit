cmake_minimum_required(VERSION 3.10)
project(minibit_compressor CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(RLE STATIC
    rle.cpp
    rle.h
)

add_library(PCE STATIC
    pce.cpp
    pce.h
)
target_include_directories(RLE PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_include_directories(PCE PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)



add_executable(minibit_compressor main.cpp)

target_link_libraries(minibit_compressor PUBLIC
    RLE
)
target_link_libraries(minibit_compressor PUBLIC
    PCE
)

