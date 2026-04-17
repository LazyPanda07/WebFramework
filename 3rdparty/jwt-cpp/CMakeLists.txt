cmake_minimum_required(VERSION 3.27.0)

set(PLATFORM_NAME "")
set(JWT_CPP_VERSION 0.7.2)
set(JWT_BUILD_EXAMPLES OFF)

project(jwt-cpp)

if (UNIX)
	if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL aarch64)
		set(PLATFORM_NAME LinuxARM)
	else()
		set(PLATFORM_NAME Linux)
	endif()
	
	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		set(PLATFORM_NAME Android)
	endif()
elseif (WIN32)
	set(PLATFORM_NAME Windows)
endif()

set(OPENSSL_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../Networks/vendor/OpenSSL/${PLATFORM_NAME})

include(FetchContent)

FetchContent_Declare(
	${PROJECT_NAME}
	GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
	GIT_TAG v${JWT_CPP_VERSION}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

# TODO: license
