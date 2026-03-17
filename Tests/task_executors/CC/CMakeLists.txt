cmake_minimum_required(VERSION 3.27.0)

project(cc_task_executors)

add_library(
	${PROJECT_NAME} SHARED
	src/c_test_task_executor.c
	src/c_executor.c
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
