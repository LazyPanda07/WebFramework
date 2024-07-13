cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)
set(WEB_FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/..)
set(WebFrameworkSDK ${WEB_FRAMEWORK_LIBRARY_DIR})
set(GTEST_LIB_DIR ${CMAKE_SOURCE_DIR}/gtest/lib/)
set(
	LIBS
    Networks
    HTTP
    JSON
    SocketStreams
    UtilityLibrary
    ssl
    crypto
)
set(
    WEB_FRAMEWORK_LIBRARY_LIB_DIRS 
    ${WEB_FRAMEWORK_LIBRARY_DIR}/lib/ 
    ${WEB_FRAMEWORK_LIBRARY_DIR}/lib/vendor/OpenSSL/
    ${WEB_FRAMEWORK_LIBRARY_DIR}/lib/vendor/sqlite3/
)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
option(CXX_API "" OFF)
option(CSHARP_API "" OFF)

add_definitions(-DSTART_CORE_SERVER_FILE="start_core_server.txt")
add_definitions(-DSTART_PROXY_SERVER_FILE="start_proxy_server.txt")
add_definitions(-DSTART_PROXY_HTTPS_SERVER_FILE="start_proxy_https_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9090_SERVER_FILE="start_load_balancer_9090_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9091_SERVER_FILE="start_load_balancer_9091_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9092_SERVER_FILE="start_load_balancer_9092_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9093_SERVER_FILE="start_load_balancer_9093_server.txt")

if(WIN32)
	set(LIBS ${LIBS} crypt32)

    add_definitions(-D_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
endif(WIN32)

if (${CXX_API})
    add_definitions(-DCXX_API)
endif()

project(Tests)

find_package(GTest)

if(NOT ${GTest_FOUND})
	set(GTEST_BOTH_LIBRARIES gtest gtest_main)
	set(GTEST_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/gtest/include/)
endif()

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(load_balancer)
add_subdirectory(proxy)
add_subdirectory(default_https_server)
add_subdirectory(api)

install(DIRECTORY configs/ DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY scripts/ DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY templates DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY assets DESTINATION ${CMAKE_INSTALL_PREFIX})

install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_DIR}/certificates DESTINATION ${CMAKE_INSTALL_PREFIX})

if (UNIX)
    set(SHARED_LIBRARY_WEB_FRAMEWORK_PATH ${WEB_FRAMEWORK_LIBRARY_DIR}/lib/libWebFrameworkLibrary.so)
else()
    set(SHARED_LIBRARY_WEB_FRAMEWORK_PATH ${WEB_FRAMEWORK_LIBRARY_DIR}/dll/WebFrameworkLibrary.dll)
endif()

if (EXISTS ${SHARED_LIBRARY_WEB_FRAMEWORK_PATH})
    install(FILES ${SHARED_LIBRARY_WEB_FRAMEWORK_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
