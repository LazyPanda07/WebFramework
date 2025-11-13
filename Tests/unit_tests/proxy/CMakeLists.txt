cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(ProxyCore)

add_executable(
	${PROJECT_NAME}
	main.cpp
	utilities/utilities.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	utilities
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	UtilityLibrary
	Networks
	gtest
	gtest_main
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
