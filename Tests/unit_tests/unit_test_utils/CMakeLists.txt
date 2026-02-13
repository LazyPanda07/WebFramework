cmake_minimum_required(VERSION 3.27.0)

set(REPROC_TAG v14.2.5)

FetchContent_Declare(
	reproc++
	GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
	GIT_TAG ${REPROC_TAG}
)

set(REPROC++ ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(reproc++)

set(CMAKE_CXX_STANDARD 20)

project(unit_test_utils)

add_library(
	${PROJECT_NAME}	STATIC
	src/ProcessWrapper.cpp
	src/UnitTestUtils.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	include
)

target_link_libraries(
	${PROJECT_NAME} PUBLIC
	reproc++
)
