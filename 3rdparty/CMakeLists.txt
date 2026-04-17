cmake_minimum_required(VERSION 3.27.0)

project(3rdparty)

add_subdirectory(cmark-gfm)
add_subdirectory(rabbitmq-c)
add_subdirectory(jwt-cpp)

if (${WITH_STACKTRACE})
	add_subdirectory(boost)
endif()

if (NOT "${COMPILE_TARGET}" STREQUAL "Android")
	if (${WITH_PYTHON_EXECUTORS})
		add_subdirectory(pybind11)
	endif()

	if (${WITH_DOTNET_EXECUTORS})
		add_subdirectory(DotNetRuntime)
	endif()
endif()
