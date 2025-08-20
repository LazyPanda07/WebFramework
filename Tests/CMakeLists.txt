cmake_minimum_required(VERSION 3.27.0)

include(cmake/enums.cmake)

set(WEB_FRAMEWORK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)
set(WEB_FRAMEWORK_SDK ${WEB_FRAMEWORK_LIBRARY_DIR})
set(GTEST_LIB_DIR ${CMAKE_SOURCE_DIR}/gtest/lib/)
set(LOCALIZATION_BUILD_TARGET build_release_localization)
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
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
option(CC_API "" OFF)
option(CXX_API "" OFF)
option(CSHARP_API "" OFF)
option(FLUTTER_API "" OFF)
option(BUILD_TESTS "" ON)
set(ALLOWED_BUILD_EXECUTORS_VALUES "CXX" "CC" "NONE")
enum_option(BUILD_EXECUTORS CXX ALLOWED_BUILD_EXECUTORS_VALUES "Build executors with specific language API")

add_definitions(-DSTART_CORE_SERVER_FILE="start_core_server.txt")
add_definitions(-DSTART_PROXY_SERVER_FILE="start_proxy_server.txt")
add_definitions(-DSTART_PROXY_HTTPS_SERVER_FILE="start_proxy_https_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9090_SERVER_FILE="start_load_balancer_9090_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9091_SERVER_FILE="start_load_balancer_9091_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9092_SERVER_FILE="start_load_balancer_9092_server.txt")
add_definitions(-DSTART_LOAD_BALANCER_9093_SERVER_FILE="start_load_balancer_9093_server.txt")
add_definitions(-DLARGE_FILE_NAME="large_file.bin")

if (UNIX)
    set(SHARED_LIBRARY_WEB_FRAMEWORK_PATH ${WEB_FRAMEWORK_LIBRARY_DIR}/lib/libWebFramework.so)
else()
    set(SHARED_LIBRARY_WEB_FRAMEWORK_PATH ${WEB_FRAMEWORK_LIBRARY_DIR}/dll/WebFramework.dll)
endif()

if(WIN32)
	set(LIBS ${LIBS} crypt32)
endif(WIN32)

if (${FLUTTER_API})
    add_definitions(-DFLUTTER_API)
endif()

project(Tests)

if(UNIX)
	file(CHMOD ${WEB_FRAMEWORK_LIBRARY_DIR}/assets/LocalizationUtils PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
endif(UNIX)

if ((NOT ${CMAKE_HOST_SYSTEM_PROCESSOR} EQUAL ${CMAKE_SYSTEM_PROCESSOR}) AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    add_definitions(-D__AARCH64__)
endif()

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

find_package(GTest)

if(NOT ${GTest_FOUND})
	set(GTEST_BOTH_LIBRARIES gtest gtest_main)
	set(GTEST_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/gtest/include/)
endif()

if (NOT ${BUILD_EXECUTORS} STREQUAL "NONE")
    include(${CMAKE_SOURCE_DIR}/cmake/generate_json.cmake)

    add_subdirectory(executors)

    string(TOLOWER ${BUILD_EXECUTORS} API_TYPE)

    set(REPLACES "{API_TYPE}" ${API_TYPE})

    generate_json(settings/web.json ${CMAKE_INSTALL_PREFIX}/web.json REPLACES)
    generate_json(settings/load_balancer_web.json ${CMAKE_INSTALL_PREFIX}/load_balancer_web.json REPLACES)

    install(DIRECTORY configs/ DESTINATION .)
    install(DIRECTORY scripts/ DESTINATION .)
    install(DIRECTORY templates DESTINATION .)
    install(DIRECTORY assets DESTINATION .)
    
    install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_DIR}/certificates DESTINATION .)
    
    install(FILES settings/default_https_server_web.json DESTINATION .)
endif()

if (${BUILD_TESTS})
    add_subdirectory(servers)
    add_subdirectory(tests)
    add_subdirectory(api)
endif()

add_custom_target(
	build_tests_localization ALL
	COMMAND ${CMAKE_MAKE_PROGRAM} ${LOCALIZATION_BUILD_TARGET}
)

add_custom_command(
	TARGET build_tests_localization
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${WEB_FRAMEWORK_LIBRARY_DIR}/localization_modules.json ${CMAKE_INSTALL_PREFIX}/localization_modules.json
)
