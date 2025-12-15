cmake_minimum_required(VERSION 3.27.0)

project(hello_executors)

if (${CXX_SAMPLE})
	add_subdirectory(cxx)
endif()

if (${CC_SAMPLE})
	add_subdirectory(cc)
endif()

if (${PYTHON_SAMPLE})
	add_subdirectory(python)
endif()

if (${CSHARP_SAMPLE})
	add_subdirectory(csharp)
endif()

install(DIRECTORY configs DESTINATION .)
