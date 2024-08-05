cmake_minimum_required(VERSION 3.27.0)

project(flutter)

if (UNIX)
	set(BUILD_PLATFORM linux)
else()
	set(BUILD_PLATFORM windows)
endif()

add_subdirectory(flutter_api)
