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
	-Wl,--whole-archive
	JSON
	-Wl,--no-whole-archive
	HTTP
	SocketStreams
	gtest
	gtest_main
	${3RDPARTY_LIBRARIES}
	WebFramework::CXX_API
	unit_test_utils
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)

add_test(NAME ${PROJECT_NAME} COMMAND ProxyCore --load_balancer_run_arguments LoadBalancerServer --proxy_run_arguments ProxyServer --port 15000 WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME ${PROJECT_NAME}HTTPS COMMAND ProxyCore --load_balancer_run_arguments LoadBalancerServer --proxy_run_arguments ProxyServer --port 15001 --useHTTPS WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
