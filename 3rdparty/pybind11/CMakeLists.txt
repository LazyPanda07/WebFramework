cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_TAG v3.0.1)
set(PYBIND11_FINDPYTHON OFF)

if (UNIX)
    set(PYTHON_EXECUTABLE /usr/bin/python3 CACHE STRING "Path to Python executable")
else()
    set(PYBIND11_FINDPYTHON OFF)

    find_package(Python REQUIRED)

    set(PYTHON_EXECUTABLE ${Python_EXECUTABLE} CACHE STRING "Path to Python executable")
endif()

if (NOT DEFINED ${PYTHON_MINOR_VERSION})
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} --version
        OUTPUT_VARIABLE PYTHON_FULL_VERSION
    )

    message(STATUS "Full Python version: ${PYTHON_FULL_VERSION}")

    string(REGEX REPLACE "Python [0-9]+\\.([0-9]+).*" "\\1" PYTHON_MINOR_VERSION "${PYTHON_FULL_VERSION}")
endif()

message(STATUS "Python minor version: ${PYTHON_MINOR_VERSION}")

project(external_pybind11)

include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG ${PYBIND11_TAG}
)

FetchContent_MakeAvailable(pybind11)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(PYTHON_TEMPORARY_DIRECTORY ${pybind11_SOURCE_DIR}/../python)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${Python_INCLUDE_DIRS} ${PYTHON_TEMPORARY_DIRECTORY}/include
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_TEMPORARY_DIRECTORY}/lib/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Python_LIBRARIES} ${PYTHON_TEMPORARY_DIRECTORY}/lib/
)

list(APPEND ADDITIONAL_LIBS ${Python_LIBRARIES} pybind11::embed)

list(APPEND ADDITIONAL_INCLUDES ${PYTHON_TEMPORARY_DIRECTORY}/include)
list(APPEND ADDITIONAL_INCLUDES ${pybind11_SOURCE_DIR}/include)
list(APPEND ADDITIONAL_LIB_DIRS ${PYTHON_TEMPORARY_DIRECTORY}/lib)

set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} CACHE INTERNAL "")
set(ADDITIONAL_INCLUDES ${ADDITIONAL_INCLUDES} CACHE INTERNAL "")
set(ADDITIONAL_LIB_DIRS ${ADDITIONAL_LIB_DIRS} CACHE INTERNAL "")
