cmake_minimum_required(VERSION 3.27.0)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(WEB_FRAMEWORK_BRANCH "main" CACHE STRING "")
set(AS_DEPENDENCY ON)
set(BUILD_PYTHON_API OFF)
set(BUILD_CSHARP_API OFF)
set(WITH_PYTHON_EXECUTORS OFF)
set(WITH_DOTNET_EXECUTORS OFF)
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(HTTP_ONLY ON CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

project(as_dependency)

add_executable(
	${PROJECT_NAME}
	src/main.cpp
)

if (UNIX)
	target_link_options(
		${PROJECT_NAME} PRIVATE
		-rdynamic
	)
endif()

FetchContent_Declare(
	WebFramework
	GIT_REPOSITORY https://github.com/LazyPanda07/WebFramework.git
	GIT_TAG ${WEB_FRAMEWORK_BRANCH}
)
FetchContent_Declare(
	GTest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)
FetchContent_Declare(
	curl
	GIT_REPOSITORY https://github.com/curl/curl.git
	GIT_TAG curl-8_19_0
)

FetchContent_MakeAvailable(WebFramework)
FetchContent_MakeAvailable(GTest)
FetchContent_MakeAvailable(curl)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	CXX_API
	GTest::gtest
	GTest::gtest_main
	CURL::libcurl
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
