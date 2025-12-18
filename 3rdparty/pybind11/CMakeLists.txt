cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_VERSION 3.0.1)
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
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v${PYBIND11_VERSION}
)

FetchContent_MakeAvailable(pybind11)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(PYTHON_TEMPORARY_DIRECTORY ${pybind11_SOURCE_DIR}/../python)

file(COPY ${Python_INCLUDE_DIRS} DESTINATION ${PYTHON_TEMPORARY_DIRECTORY})
file(MAKE_DIRECTORY ${PYTHON_TEMPORARY_DIRECTORY}/lib/)
file(COPY ${Python_LIBRARIES} DESTINATION ${PYTHON_TEMPORARY_DIRECTORY}/lib/)

list(APPEND ADDITIONAL_LIBS ${Python_LIBRARIES} pybind11::embed)

list(APPEND ADDITIONAL_INCLUDES ${PYTHON_TEMPORARY_DIRECTORY}/include)
list(APPEND ADDITIONAL_INCLUDES ${pybind11_SOURCE_DIR}/include)
list(APPEND ADDITIONAL_LIB_DIRS ${PYTHON_TEMPORARY_DIRECTORY}/lib)

set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} CACHE INTERNAL "")
set(ADDITIONAL_INCLUDES ${ADDITIONAL_INCLUDES} CACHE INTERNAL "")
set(ADDITIONAL_LIB_DIRS ${ADDITIONAL_LIB_DIRS} CACHE INTERNAL "")
