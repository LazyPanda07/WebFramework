cmake_minimum_required(VERSION 3.27.0)

project(WebFrameworkCore)

add_library(
	${PROJECT_NAME} STATIC
	src/Utility/JSONSettingsParser.cpp
	src/Exceptions/BaseWebFrameworkException.cpp
	src/Exceptions/DatabaseException.cpp
	src/Exceptions/SQLite3Exception.cpp
	src/WebNetwork/HTTPRequest.cpp
	src/WebNetwork/HTTPResponse.cpp
	src/Managers/SessionsManager.cpp
	src/SQLite3/SQLiteDatabase.cpp
	src/SQLite3/SQLiteDatabaseModel.cpp
	src/SQLite3/SQLiteResult.cpp
	src/SQLite3/SQLiteManager.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	src/
	vendor/sqlite3/include/
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)
