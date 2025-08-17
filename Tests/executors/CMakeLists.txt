cmake_minimum_required(VERSION 3.27.0)

project(executors)

if (${BUILD_EXECUTORS} STREQUAL "cxx")
	add_subdirectory(cxx)
endif()
