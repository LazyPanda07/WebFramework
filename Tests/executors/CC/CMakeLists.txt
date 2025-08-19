cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_CC_API ON)

project(cc_executors)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

add_subdirectory(core)
add_subdirectory(database)
add_subdirectory(load_balancer)
