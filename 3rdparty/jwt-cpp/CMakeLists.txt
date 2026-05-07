cmake_minimum_required(VERSION 3.27.0)

set(PLATFORM_NAME "")
set(JWT_CPP_VERSION 0.7.2)
set(JWT_BUILD_EXAMPLES OFF)

project(jwt-cpp)

add_definitions(-DNOMINMAX)

if (UNIX)
	if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
		set(PLATFORM_NAME LinuxARM)
	else()
		set(PLATFORM_NAME Linux)
	endif()

	if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
		set(PLATFORM_NAME Android)
	endif()
elseif (WIN32)
	set(PLATFORM_NAME Windows)
endif()

set(OPENSSL_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../Networks/vendor/OpenSSL/${PLATFORM_NAME})

if ("${PLATFORM_NAME}" STREQUAL "Android")
	set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
	set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.a)
	set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.a)

	add_library(OpenSSL::SSL UNKNOWN IMPORTED)
	set_target_properties(OpenSSL::SSL PROPERTIES
	    IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
	    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
	)

	add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
	set_target_properties(OpenSSL::Crypto PROPERTIES
	    IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
	    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
	)
endif()

include(FetchContent)

FetchContent_Declare(
	${PROJECT_NAME}
	GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
	GIT_TAG v${JWT_CPP_VERSION}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	install(DIRECTORY license/ DESTINATION license)
endif()
