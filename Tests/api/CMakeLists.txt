cmake_minimum_required(VERSION 3.27.0)

project(API)

if (${PYTHON_API})
	add_subdirectory(python)
endif()

if (${CC_API})
	add_subdirectory(CC)
endif()

if (${CXX_API})
	add_subdirectory(CXX)
endif()

if (${CSHARP_API})
	add_subdirectory(CSharp)
endif()

if (${FLUTTER_API})
	add_subdirectory(flutter)
endif()
