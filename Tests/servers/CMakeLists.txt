cmake_minimum_required(VERSION 3.27.0)

project(servers)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

add_subdirectory(core)
add_subdirectory(default_https_server)
add_subdirectory(load_balancer)
add_subdirectory(proxy)
