cmake_minimum_required(VERSION 3.27.0)

include(FetchContent)

set(REPROC_TAG v14.2.5)
set(NETWORKS_VERSION 1.22.6)

FetchContent_Declare(
	reproc++
	GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
	GIT_TAG ${REPROC_TAG}
)
FetchContent_Declare(
	ExternalNetworks
	GIT_REPOSITORY https://github.com/LazyPanda07/Networks.git
	GIT_TAG v${NETWORKS_VERSION}
)

set(REPROC++ ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(reproc++)
FetchContent_MakeAvailable(ExternalNetworks)

set(CMAKE_CXX_STANDARD 20)

project(task_broker_test)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	GTest::gtest
	GTest::gtest_main
	reproc++
	Networks
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
