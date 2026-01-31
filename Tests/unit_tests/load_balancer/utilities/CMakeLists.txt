cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(LoadBalancerCore)

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
	Networks
	UtilityLibrary
	-Wl,--whole-archive
	JSON
	-Wl,--no-whole-archive
	HTTP
	SocketStreams
	gtest 
	gtest_main
	${3RDPARTY_LIBRARIES}
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
