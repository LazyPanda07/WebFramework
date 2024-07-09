cmake_minimum_required(VERSION 3.27.0)

project(API)

add_subdirectory(python)

if (${CXX_API})
	add_subdirectory(CXX)
elseif (${CSHARP_API})
	add_subdirectory(CSharp)
endif()
