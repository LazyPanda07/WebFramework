cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_BUILD_TYPE Release CACHE STRING "")
option(PYTHON_SAMPLE "Build Python sample" ON)
option(CXX_SAMPLE "Build C++ sample" ON)
option(CC_SAMPLE "Build C sample" ON)
option(CSHARP_SAMPLE "Build C# sample" ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")

project(samples)

set(WEB_FRAMEWORK_SDK ${PROJECT_SOURCE_DIR}/../WebFrameworkLibrary CACHE STRING "")
set(WEB_FRAMEWORK_DIRECTORY ${PROJECT_SOURCE_DIR}/../WebFrameworkLibrary CACHE STRING "")

include(${WEB_FRAMEWORK_DIRECTORY}/WebFrameworkConfig.cmake)

add_subdirectory(hello_executor)

if (${PYTHON_SAMPLE})
	add_subdirectory(hello_python)
endif()

if (${CXX_SAMPLE})
	add_subdirectory(hello_cpp)
endif()

if (${CC_SAMPLE})
	add_subdirectory(hello_c)
endif()

if (${CSHARP_SAMPLE})
	add_subdirectory(hello_csharp)
endif()
