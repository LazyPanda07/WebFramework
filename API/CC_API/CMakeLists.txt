cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/WebFrameworkLibrary CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_API)

add_library(
	${PROJECT_NAME} STATIC
	src/dummy.c
)

install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/api/cc/include FILES_MATCHING PATTERN "*.h")
