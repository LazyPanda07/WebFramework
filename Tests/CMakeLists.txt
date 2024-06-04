cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)
set(WEB_FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/..)
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

add_definitions(-DSTART_SERVER_FILE="start_server.txt")

if(WIN32)
	set(LIBS ${LIBS} crypt32)
endif(WIN32)

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

install(DIRECTORY configs/ DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY scripts/ DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY templates DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY assets DESTINATION ${CMAKE_INSTALL_PREFIX})

install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_DIR}/certificates DESTINATION ${CMAKE_INSTALL_PREFIX})
