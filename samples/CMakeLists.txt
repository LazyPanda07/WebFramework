cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_BUILD_TYPE Release CACHE STRING "")

project(samples)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(WEB_FRAMEWORK_SDK ../WebFrameworkLibrary)
include(../WebFrameworkLibrary/WebFramework.cmake)

add_subdirectory(hello_executor)
add_subdirectory(hello_python)
