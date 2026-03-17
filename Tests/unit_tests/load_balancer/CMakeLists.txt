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
	unit_test_utils
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

add_test(NAME load_balancer_default COMMAND LoadBalancerCore --load_balancer_run_arguments LoadBalancerServer --config load_balancer_config.json --port 9090 --runtimes python WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME load_balancer_https_servers COMMAND LoadBalancerCore --load_balancer_run_arguments LoadBalancerServer --config load_balancer_config.json --port 9091 --serversHTTPS --runtimes python WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME load_balancer_https COMMAND LoadBalancerCore --load_balancer_run_arguments LoadBalancerServer --config load_balancer_config_https.json --port 9092 --useHTTPS --runtimes python WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME load_balancer_https_https_servers COMMAND LoadBalancerCore --load_balancer_run_arguments LoadBalancerServer --config load_balancer_config_https.json --port 9093 --useHTTPS --serversHTTPS --runtimes python WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
add_test(NAME load_balancer_custom_heuristic COMMAND LoadBalancerCore --load_balancer_run_arguments LoadBalancerServer --config load_balancer_config.json --port 9094 --custom_heuristic --runtimes python WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
