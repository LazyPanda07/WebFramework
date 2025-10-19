cmake_minimum_required(VERSION 3.27.0)

project(executors)

if (${BUILD_EXECUTORS} STREQUAL "CXX")
	add_subdirectory(CXX)
endif()

if (${BUILD_EXECUTORS} STREQUAL "CC")
	add_subdirectory(CC)
endif()

if (${BUILD_EXECUTORS} STREQUAL "Python")
	add_subdirectory(Python)
endif()
