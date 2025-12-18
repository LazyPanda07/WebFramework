cmake_minimum_required(VERSION 3.27.0)

project(cxx_hello_executor)

add_library(
	${PROJECT_NAME} SHARED
	src/CXXHelloExecutor.cpp
)

if (UNIX AND ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	add_definitions(-D__ANDROID__)

	target_compile_options(
		${PROJECT_NAME} PRIVATE 
		-fexceptions -frtti
	)
endif()

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION executors)
install(DIRECTORY settings/ DESTINATION executors)
