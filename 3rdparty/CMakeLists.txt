cmake_minimum_required(VERSION 3.27.0)

project(3rdparty)

add_subdirectory(cmark-gfm)

if (${WITH_STACKTRACE})
	add_subdirectory(boost)
endif()

if (${WITH_PYTHON_EXECUTORS} AND ${COMPILE_TARGET} STREQUAL Linux OR ${COMPILE_TARGET} STREQUAL Windows)
	add_subdirectory(pybind11)
endif()

if (${WITH_DOT_NET_EXECUTORS} AND ${COMPILE_TARGET} STREQUAL Linux OR ${COMPILE_TARGET} STREQUAL Windows)
	add_subdirectory(DotNetRuntime)
endif()
