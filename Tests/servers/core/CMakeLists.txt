cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_INCLUDE}
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
