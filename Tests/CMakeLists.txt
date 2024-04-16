cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(WEB_FRAMEWORK_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)
set(
	LIBS
	WebFramework
	BaseTCPServer
	JSON
	FileManager
	ThreadPool
	Log
	HTTP
	Networks
	SocketStreams
	UtilityLibrary 
	sqlite3
	ssl
	crypto
)

if (UNIX)
	add_definitions(-D__LINUX__)
else ()
	set(LIBS ${LIBS} crypt32)
endif (UNIX)

project(Tests)

add_executable(
	${PROJECT_NAME}
	main.cpp
	utilities/utilities.cpp
	cases/Requests.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	utilities/
	${CMAKE_SOURCE_DIR}/../WebFramework/src/
	${CMAKE_SOURCE_DIR}/../BaseTCPServer/src/
	${CMAKE_SOURCE_DIR}/../Networks/src/
	${CMAKE_SOURCE_DIR}/../Networks/HTTP/src/
	${CMAKE_SOURCE_DIR}/../Networks/HTTP/JSON/src/
	${CMAKE_SOURCE_DIR}/../FileManager/src/
	${CMAKE_SOURCE_DIR}/../ThreadPool/src/
	${CMAKE_SOURCE_DIR}/../Log/src/
	${CMAKE_SOURCE_DIR}/../SHA256/src/
	${CMAKE_SOURCE_DIR}/../INIParser/src/
	${CMAKE_SOURCE_DIR}/../Localization/src/
	${CMAKE_SOURCE_DIR}/../Networks/SocketStreams/src/
	${CMAKE_SOURCE_DIR}/../Networks/vendor/OpenSSL/include/
	${CMAKE_SOURCE_DIR}/../WebFramework/vendor/sqlite3/include/
	gtest/include/
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_PATH}/lib/
	gtest/lib/
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${LIBS}
	gtest
	gtest_main
)

add_subdirectory(server)

if (UNIX)
	install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_PATH}/lib/ DESTINATION ${CMAKE_BINARY_DIR}/bin FILES_MATCHING PATTERN "*.so")
else ()
	install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_PATH}/dll/ DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif (UNIX)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(DIRECTORY configs/ DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_PATH}/certificates DESTINATION ${CMAKE_BINARY_DIR}/bin)
