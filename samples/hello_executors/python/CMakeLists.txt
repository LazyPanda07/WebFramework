cmake_minimum_required(VERSION 3.27.0)

project(python_hello_executor)

add_custom_target(
	${PROJECT_NAME} ALL
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/executors
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/python_hello_executor.py ${CMAKE_INSTALL_PREFIX}/executors
)

install(DIRECTORY settings/ DESTINATION executors)
