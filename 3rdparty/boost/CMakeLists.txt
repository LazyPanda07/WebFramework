cmake_minimum_required(VERSION 3.27.0)

set(BOOST_VERSION 1.89.0)
set(BOOST_TAG boost-${BOOST_VERSION})

project(external_boost)

find_package(Boost QUIET COMPONENTS stacktrace)

if (NOT ${Boost_FOUND})
    set(FETCHCONTENT_QUIET OFF CACHE BOOL "")
    set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "")
    set(BOOST_STACKTRACE_ENABLE_NOOP OFF)
    set(BOOST_STACKTRACE_ENABLE_FROM_EXCEPTION OFF)

    include(FetchContent)

    if (UNIX AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
        set(BOOST_STACKTRACE_ENABLE_BACKTRACE ON)
    elseif(WIN32)
        set(BOOST_STACKTRACE_ENABLE_WINDBG ON)
        set(BOOST_STACKTRACE_ENABLE_BACKTRACE OFF)
    endif()

    FetchContent_Declare(
        ${PROJECT_NAME}
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG ${BOOST_TAG}
    )

    FetchContent_MakeAvailable(${PROJECT_NAME})
endif()

if (UNIX AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    list(APPEND ADDITIONAL_LIBS boost_stacktrace_backtrace dl backtrace)
elseif(WIN32)
    list(APPEND ADDITIONAL_LIBS boost_stacktrace_windbg Dbghelp)
endif()

set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} CACHE INTERNAL "")
