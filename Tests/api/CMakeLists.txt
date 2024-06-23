cmake_minimum_required(VERSION 3.27.0)

project(API)

add_subdirectory(python_api)

if (${CXX_API})
	add_subdirectory(cxx_api)
endif()
