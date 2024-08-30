cmake_minimum_required(VERSION 3.27.0)

project(samples)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(WEB_FRAMEWORK_SDK ../WebFrameworkLibrary)
include(${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary/WebFramework.cmake)

add_subdirectory(hello)
