cmake_minimum_required(VERSION 3.27.0)

project(CC)

set(WEB_FRAMEWORK_CC_API ON)

add_subdirectory(CC_API)
add_subdirectory(CC_Server)
add_subdirectory(CC_LoadBalancerServer)
add_subdirectory(CC_ProxyServer)
