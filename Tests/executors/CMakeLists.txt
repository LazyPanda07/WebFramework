cmake_minimum_required(VERSION 3.27.0)

project(executors)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

if (${BUILD_CXX_EXECUTORS})
	add_subdirectory(cxx)
endif()
