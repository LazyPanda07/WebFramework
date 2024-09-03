cmake_minimum_required(VERSION 3.27.0)

project(samples)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(WEB_FRAMEWORK_SDK ../WebFrameworkLibrary)
include(../WebFrameworkLibrary/WebFramework.cmake)

add_subdirectory(hello)
