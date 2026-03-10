cmake_minimum_required(VERSION 3.27.0)

project(3rdparty)

add_subdirectory(cmark-gfm)

if (${WITH_STACKTRACE})
	add_subdirectory(boost)
endif()

if (${WITH_AMQP})
	add_subdirectory(rabbitmq-c)
endif()

if (${WITH_PYTHON_EXECUTORS})
	if (${COMPILE_TARGET} STREQUAL Linux OR ${COMPILE_TARGET} STREQUAL Windows)
		add_subdirectory(pybind11)
	endif()
endif()

if (${WITH_DOTNET_EXECUTORS})
	if (${COMPILE_TARGET} STREQUAL Linux OR ${COMPILE_TARGET} STREQUAL Windows)
		add_subdirectory(DotNetRuntime)
	endif()
endif()
