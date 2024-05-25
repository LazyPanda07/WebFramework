cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)
set(WEB_FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/..)
set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/gtest/include/)
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

add_definitions(-DSTART_SERVER_FILE="start_server.txt")

if (WIN32)
	set(LIBS ${LIBS} crypt32)
endif (WIN32)

project(Tests)

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(load_balancer)

if (UNIX)
	install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_DIR}/lib/ DESTINATION ${CMAKE_BINARY_DIR}/bin FILES_MATCHING PATTERN "*.so")
else ()
	install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_DIR}/dll/ DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif (UNIX)

install(DIRECTORY configs/ DESTINATION ${CMAKE_BINARY_DIR}/bin)
install(DIRECTORY scripts/ DESTINATION ${CMAKE_BINARY_DIR}/bin)
install(DIRECTORY templates DESTINATION ${CMAKE_BINARY_DIR}/bin)
install(DIRECTORY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_DIR}/certificates DESTINATION ${CMAKE_BINARY_DIR}/bin)
