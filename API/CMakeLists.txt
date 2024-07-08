cmake_minimum_required(VERSION 3.27.0)

project(API)

if(${WEB_FRAMEWORK_LIBRARY_TYPE} STREQUAL SHARED)
	add_subdirectory(CXX_API)
	add_subdirectory(python_api)
	add_subdirectory(CSharp_API)
endif()
