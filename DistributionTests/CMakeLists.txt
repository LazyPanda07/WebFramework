cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE STRING "")

project(distribution)

include(FetchContent)

option(TEST_PYTHON "" OFF)
option(TEST_CC "" OFF)

if (${TEST_PYTHON})
	add_subdirectory(python)
endif()

if (${TEST_CC})
	add_subdirectory(cc)
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/configs/ DESTINATION bin)

if (UNIX)
	set(HELLO_EXECUTOR_URL https://github.com/LazyPanda07/WebFramework/releases/download/Assets/linux.zip)
else()
	set(HELLO_EXECUTOR_URL https://github.com/LazyPanda07/WebFramework/releases/download/Assets/windows.zip)
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
