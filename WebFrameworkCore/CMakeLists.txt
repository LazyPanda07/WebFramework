cmake_minimum_required(VERSION 3.27.0)

project(WebFrameworkCore)

add_library(
	${PROJECT_NAME} STATIC
	src/Exceptions/BaseWebFrameworkException.cpp
	src/Managers/SessionsManager.cpp
	src/Utility/Stopwatch.cpp
)

if (${WITH_UNITY_BUILD})
	set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
endif()

target_include_directories(
	${PROJECT_NAME} PUBLIC
	src/
	${PROJECT_SOURCE_DIR}/../Networks/src/
	${PROJECT_SOURCE_DIR}/../Networks/HTTP/src/
	${PROJECT_SOURCE_DIR}/../Networks/HTTP/JSON/src/
	${PROJECT_SOURCE_DIR}/../Networks/SocketStreams/src/
	${PROJECT_SOURCE_DIR}/../Networks/vendor/OpenSSL/include/
	${PROJECT_SOURCE_DIR}/../Log/src/
	${PROJECT_SOURCE_DIR}/../Localization/src/
	${PROJECT_SOURCE_DIR}/../BaseTCPServer/src/
	${PROJECT_SOURCE_DIR}/../FileManager/src/
	${PROJECT_SOURCE_DIR}/../UtilityLibrary/include/
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)
