cmake_minimum_required(VERSION 3.27.0)

set(RABBITMQ-C_TAG 47f99dce572b7229a11b9b011f5a6fd39b93bac1)
set(BUILD_SHARED_LIBS OFF)
set(ENABLE_SSL_SUPPORT OFF)

project(rabbitmq-c)

include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}
    GIT_REPOSITORY https://github.com/alanxz/rabbitmq-c.git
    GIT_TAG ${RABBITMQ-C_TAG}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	install(FILES ${rabbitmq-c_SOURCE_DIR}/LICENSE DESTINATION license RENAME rabbitmq-c_LICENSE)
endif()
