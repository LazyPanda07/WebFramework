cmake_minimum_required(VERSION 3.27.0)

project(3rdparty)

if (${WITH_STACKTRACE})
	add_subdirectory(boost)
endif()
