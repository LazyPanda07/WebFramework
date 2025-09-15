cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_CC_API ON)

project(CC)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

add_subdirectory(CC_API_TESTS)
add_subdirectory(CC_Server)
add_subdirectory(CC_LoadBalancerServer)
add_subdirectory(CC_ProxyServer)
