cmake_minimum_required(VERSION 3.27.0)

option(BUILD_CC_API "Build CC API" ON)
option(BUILD_CXX_API "Build C++ API" ON)
option(BUILD_CSHARP_API "Build C# API" ON)
option(BUILD_PYTHON_API "Build Python API" ON)
option(BUILD_FLUTTER_API "Build Flutter API" ON)

project(API)

if (${BUILD_CC_API})
	add_subdirectory(CC_API)
endif()

if (${BUILD_CXX_API})
	add_subdirectory(CXX_API)
endif()

if (${BUILD_CSHARP_API})
	add_subdirectory(CSharp_API)
endif()

if (${BUILD_PYTHON_API})
	add_subdirectory(python_api)
endif()

if (${BUILD_FLUTTER_API})
	install(DIRECTORY web_framework_flutter_api/ DESTINATION ${CMAKE_INSTALL_PREFIX}/api/web_framework_flutter_api/)
endif()
