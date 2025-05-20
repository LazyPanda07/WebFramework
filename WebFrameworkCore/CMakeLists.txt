cmake_minimum_required(VERSION 3.27.0)

project(WebFrameworkCore)

add_library(
	${PROJECT_NAME} STATIC
	src/Exceptions/BaseWebFrameworkException.cpp
	src/Managers/SessionsManager.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	src/
	${CMAKE_SOURCE_DIR}/Networks/src/
	${CMAKE_SOURCE_DIR}/Networks/HTTP/src/
	${CMAKE_SOURCE_DIR}/Networks/HTTP/JSON/src/
	${CMAKE_SOURCE_DIR}/Networks/SocketStreams/src/
	${CMAKE_SOURCE_DIR}/Networks/vendor/OpenSSL/include/
	${CMAKE_SOURCE_DIR}/Log/src/
	${CMAKE_SOURCE_DIR}/Localization/src/
	${CMAKE_SOURCE_DIR}/BaseTCPServer/src/
	${CMAKE_SOURCE_DIR}/FileManager/src/
	${CMAKE_SOURCE_DIR}/UtilityLibrary/include/
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
