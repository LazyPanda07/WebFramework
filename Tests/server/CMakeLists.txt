cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
if (UNIX)
	add_definitions(-D__LINUX__)

	set(CMAKE_CXX_FLAGS -rdynamic)
endif (UNIX)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
	executors/HelloExecutor.cpp
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
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
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_PATH}/lib/
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)
