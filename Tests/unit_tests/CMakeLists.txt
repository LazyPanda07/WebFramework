cmake_minimum_required(VERSION 3.27.0)

set(GTEST_VERSION 1.17.0)

project(test_suites)

include(FetchContent)

FetchContent_Declare(
	gtest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v${GTEST_VERSION}
)

FetchContent_MakeAvailable(gtest)

add_subdirectory(core)
add_subdirectory(load_balancer)
add_subdirectory(proxy)
