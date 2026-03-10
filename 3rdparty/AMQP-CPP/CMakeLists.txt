cmake_minimum_required(VERSION 3.27.0)

set(AMQP-CPP_TAG 839f74f2e7cc0f3f03458ebf943d23fc1f8c5462)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(AMQP-CPP)

include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}
    GIT_REPOSITORY https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git
    GIT_TAG ${CPP_TAG}
)

FetchContent_MakeAvailable(${PROJECT_NAME})

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	install(FILES ${amqp-cpp_SOURCE_DIR}/LICENSE DESTINATION license RENAME AMQP-CPP_LICENSE)
endif()
