cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(Database)

add_library(
	${PROJECT_NAME} SHARED
	CRUDExecutor.cpp
	MultiUserExecutor.cpp
	DatabaseModel.cpp
	MultiUserDatabaseModel.cpp
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
	${WEB_FRAMEWORK_DIR}/UtilityLibrary/include/
	${WEB_FRAMEWORK_DIR}/Networks/vendor/OpenSSL/include/
	${WEB_FRAMEWORK_DIR}/WebFramework/vendor/sqlite3/include/
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
