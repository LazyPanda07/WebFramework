cmake_minimum_required(VERSION 4.0.0)

set(ZLIB_VERSION 1.3.2)
set(ZLIB_BUILD_SHARED OFF)

project(zlib)

include(FetchContent)

FetchContent_Declare(
	${PROJECT_NAME}
	GIT_REPOSITORY https://github.com/madler/zlib.git
	GIT_TAG v${ZLIB_VERSION}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

set_target_properties(zlibstatic PROPERTIES
    DEBUG_POSTFIX ""
)

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	install(DIRECTORY license/ DESTINATION license)
endif()
