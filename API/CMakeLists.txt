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
	install(
		DIRECTORY Python_API/ DESTINATION api/python_api
		PATTERN "*.vcxproj*" EXCLUDE
		PATTERN "build" EXCLUDE
		PATTERN "out" EXCLUDE
		PATTERN ".vs" EXCLUDE
	)
	install(DIRECTORY ${CXX_API_INSTALL_DESTINATION}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/api/python_api/cxx/include)
	if (UNIX)
		install(
			CODE [[
				execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/api/python_api/lib)
			]]
		)
	elseif (WIN32)
		install(
			CODE [[
				execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_INSTALL_PREFIX}/dll ${CMAKE_INSTALL_PREFIX}/api/python_api/dll)
			]]
		)
	endif()
	
	install(
		CODE [[
			execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_INSTALL_PREFIX}/license ${CMAKE_INSTALL_PREFIX}/api/python_api/license)
		]]
	)
endif()

if (${BUILD_FLUTTER_API})
	install(DIRECTORY web_framework_flutter_api/ DESTINATION ${CMAKE_INSTALL_PREFIX}/api/web_framework_flutter_api/)
endif()
