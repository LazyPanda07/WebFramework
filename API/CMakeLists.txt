cmake_minimum_required(VERSION 3.27.0)

project(API)

if(${WEB_FRAMEWORK_LIBRARY_TYPE} STREQUAL SHARED)
	add_subdirectory(CC_API)
	add_subdirectory(CXX_API)
	add_subdirectory(CSharp_API)
	add_subdirectory(python_api)
endif()

if(${WEB_FRAMEWORK_LIBRARY_TYPE} STREQUAL SHARED)
	install(DIRECTORY web_framework_flutter_api DESTINATION ${CMAKE_INSTALL_PREFIX}/api/web_framework_flutter_api/)
endif()
