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
	include/
	${PROJECT_SOURCE_DIR}/../Networks/include/
	${PROJECT_SOURCE_DIR}/../Networks/HTTP/include/
	${PROJECT_SOURCE_DIR}/../Networks/HTTP/JSON/include/
	${PROJECT_SOURCE_DIR}/../Networks/SocketStreams/include/
	${PROJECT_SOURCE_DIR}/../Networks/vendor/OpenSSL/include/
	${PROJECT_SOURCE_DIR}/../Log/include/
	${PROJECT_SOURCE_DIR}/../Localization/include/
	${PROJECT_SOURCE_DIR}/../BaseTCPServer/include/
	${PROJECT_SOURCE_DIR}/../FileManager/include/
	${PROJECT_SOURCE_DIR}/../UtilityLibrary/include/
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
