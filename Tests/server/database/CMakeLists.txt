cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(
	LIBS
	WebFramework
	${LIBS}
	sqlite3
)

if (UNIX)
	add_definitions(-D__LINUX__)
endif (UNIX)

project(Database)

add_library(
	${PROJECT_NAME} SHARED
	CRUDExecutor.cpp
	DatabaseModel.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_DIR}/WebFramework/src/
	${WEB_FRAMEWORK_DIR}/BaseTCPServer/src/
	${WEB_FRAMEWORK_DIR}/Networks/src/
	${WEB_FRAMEWORK_DIR}/Networks/HTTP/src/
	${WEB_FRAMEWORK_DIR}/Networks/HTTP/JSON/src/
	${WEB_FRAMEWORK_DIR}/Networks/SocketStreams/src/
	${WEB_FRAMEWORK_DIR}/FileManager/src/
	${WEB_FRAMEWORK_DIR}/ThreadPool/src/
	${WEB_FRAMEWORK_DIR}/Log/src/
	${WEB_FRAMEWORK_DIR}/SHA256/src/
	${WEB_FRAMEWORK_DIR}/INIParser/src/
	${WEB_FRAMEWORK_DIR}/Localization/src/
	${WEB_FRAMEWORK_DIR}/Networks/vendor/OpenSSL/include/
	${WEB_FRAMEWORK_DIR}/WebFramework/vendor/sqlite3/include/
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_DIR}/lib/
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)
