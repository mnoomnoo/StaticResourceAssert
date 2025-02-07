
cmake_minimum_required(VERSION 3.20)


project(
    StaticResourceAssert
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if( WIN32 )
    add_compile_options(/utf-8)
endif()

add_subdirectory(StaticResourceAssert)
add_subdirectory(StaticResourceAssertUnitTests)

