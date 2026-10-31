cmake_minimum_required(VERSION 4.0.0)

set(BROTLI_VERSION 1.2.1)
set(BUILD_SHARED_LIBS OFF)

project(brotli)

include(FetchContent)

FetchContent_Declare(
	${PROJECT_NAME}
	GIT_REPOSITORY https://github.com/google/brotli.git
	GIT_TAG v${BROTLI_VERSION}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	install(DIRECTORY license/ DESTINATION license)
endif()
