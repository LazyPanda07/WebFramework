cmake_minimum_required(VERSION 3.27.0)

project(API)

if(${SHARED_WEB_FRAMEWORK})
	add_subdirectory(CXX_API)
	add_subdirectory(python_api)
endif()
