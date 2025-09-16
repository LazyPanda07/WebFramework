cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE STRING "")

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

install(DIRECTORY ${CMAKE_SOURCE_DIR}/configs/ DESTINATION bin)

if (UNIX)
	set(HELLO_EXECUTOR_URL https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_linux.zip)
else()
	set(HELLO_EXECUTOR_URL https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_windows.zip)
endif()

FetchContent_Declare(
	hello_executor
	URL ${HELLO_EXECUTOR_URL}
)

FetchContent_MakeAvailable(
	hello_executor
)

install(DIRECTORY ${hello_executor_SOURCE_DIR}/ DESTINATION bin)
install(FILES reference.json DESTINATION bin)
