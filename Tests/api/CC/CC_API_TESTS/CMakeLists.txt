cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_C_STANDARD 11)

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_API_TESTS)

add_executable(
	${PROJECT_NAME}
	main.cpp
	cases.c
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_DIR}/api/cc/include/
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	gtest
	gtest_main
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
