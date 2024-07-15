cmake_minimum_required(VERSION 3.27.0)

project(API)

add_subdirectory(python)

if (${CC_API})
	add_subdirectory(CC)
endif()

if (${CXX_API})
	add_subdirectory(CXX)
endif()

if (${CSHARP_API})
	add_subdirectory(CSharp)
endif()
