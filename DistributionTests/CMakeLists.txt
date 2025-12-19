cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")

project(distribution)

include(FetchContent)

option(TEST_PYTHON "" OFF)
option(TEST_CSHARP "" OFF)
option(TEST_DEV_CC "" OFF)
option(TEST_DEV_CXX "" OFF)

if (${TEST_PYTHON})
	add_subdirectory(python)
endif()

if (${TEST_CSHARP})
	add_subdirectory(csharp)
endif()

if (${TEST_DEV_CC})
	add_subdirectory(cc)
endif()

if (${TEST_DEV_CXX})
	add_subdirectory(cxx)
endif()

install(FILES config.json DESTINATION .)
install(FILES reference.json DESTINATION .)
