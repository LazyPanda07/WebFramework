cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CXX_API)

add_library(
	${PROJECT_NAME} STATIC
	src/dummy.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	include
)

install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION dll
)

install(DIRECTORY include DESTINATION api/cxx)
