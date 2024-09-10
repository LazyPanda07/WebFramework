cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_BUILD_TYPE Release CACHE STRING "")
option(PYTHON_SAMPLE "Build Python sample" ON)
option(CXX_SAMPLE "Build C++ sample" ON)

project(samples)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)
set(WEB_FRAMEWORK_SDK ${PROJECT_SOURCE_DIR}/../WebFrameworkLibrary)
include(${PROJECT_SOURCE_DIR}/../WebFrameworkLibrary/WebFramework.cmake)

add_subdirectory(hello_executor)

if (${PYTHON_SAMPLE})
	add_subdirectory(hello_python)
endif()

if (${CXX_SAMPLE})
	add_subdirectory(hello_cpp)
endif()
