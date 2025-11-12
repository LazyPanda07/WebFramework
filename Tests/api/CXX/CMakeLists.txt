cmake_minimum_required(VERSION 3.27.0)

project(CXX_API_TESTS)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	gtest
	gtest_main
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
