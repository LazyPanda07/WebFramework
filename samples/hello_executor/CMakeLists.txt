cmake_minimum_required(VERSION 3.27.0)

project(hello_executor)

add_library(
	${PROJECT_NAME} SHARED
	src/HelloExecutor.cpp
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

if (WIN32)
	install(
		TARGETS ${PROJECT_NAME} 
		ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
		LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/executors
	)
else()
	install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/executors)
endif()

install(DIRECTORY configs DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY settings/ DESTINATION ${CMAKE_INSTALL_PREFIX}/executors)
