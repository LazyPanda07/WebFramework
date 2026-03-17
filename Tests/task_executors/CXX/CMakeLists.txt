cmake_minimum_required(VERSION 3.27.0)

project(cxx_task_executors)

add_library(
	${PROJECT_NAME} SHARED
	src/CXXTestTaskExecutor.cpp
	src/CXXExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
