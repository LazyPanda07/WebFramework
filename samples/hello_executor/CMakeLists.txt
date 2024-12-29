cmake_minimum_required(VERSION 3.27.0)

project(hello_executor)

add_library(
	${PROJECT_NAME} SHARED
	source/HelloExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
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
