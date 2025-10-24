cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_TAG v3.0.1)

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

if (UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(web_framework_api VERSION 3.1.3)

if (UNIX)
    add_definitions(-D__LINUX__)
endif()

set(WEB_FRAMEWORK_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/../CXX_API/src" CACHE STRING "Path to include directory")

include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG ${PYBIND11_TAG}
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(pybind11)

find_package(pybind11 REQUIRED)

pybind11_add_module(
    ${PROJECT_NAME} 
    src/module.cpp
    src/Executors/PyStatelessExecutor.cpp
    src/Executors/PyStatefulExecutor.cpp
    src/Executors/PyHeavyOperationStatelessExecutor.cpp
    src/Executors/PyHeavyOperationStatefulExecutor.cpp
    src/PyChunkGenerator.cpp
    src/PyLoadBalancerHeuristic.cpp
    src/PyDynamicFunction.cpp
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    include
    cxx/include
    ${WEB_FRAMEWORK_INCLUDE_PATH}
)

if (DEFINED CUSTOM_INSTALL_PATH)
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CUSTOM_INSTALL_PATH})
else()
    if (UNIX)
        add_custom_target(
            generate_pyi ALL
            DEPENDS ${PROJECT_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

            COMMAND ${PYTHON_EXECUTABLE} -m venv .venv
            COMMAND .venv/bin/python -m pip install pybind11-stubgen
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/*.so ${CMAKE_BINARY_DIR}/.venv/lib/python3.${PYTHON_MINOR_VERSION}/site-packages
            COMMAND ${CMAKE_BINARY_DIR}/.venv/bin/pybind11-stubgen -o ${CMAKE_BINARY_DIR} ${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E rm -rf .venv
        )
    else()
        add_custom_target(
            generate_pyi ALL
            DEPENDS ${PROJECT_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

            COMMAND ${PYTHON_EXECUTABLE} -m venv .venv
            COMMAND .venv/Scripts/python -m pip install pybind11-stubgen
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/.venv/Lib/site-packages
            COMMAND ${CMAKE_BINARY_DIR}/.venv/Scripts/pybind11-stubgen -o ${CMAKE_BINARY_DIR} ${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E rm -rf .venv
        )
    endif()

    install(TARGETS ${PROJECT_NAME} DESTINATION .)
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pyi DESTINATION .)
endif()

if (EXISTS lib)
    install(DIRECTORY lib/ DESTINATION .)
endif()

if (EXISTS dll)
    install(DIRECTORY dll/ DESTINATION .)
endif()
