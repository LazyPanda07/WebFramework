cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_C_STANDARD 11)

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_LoadBalancerServer)

add_executable(
	${PROJECT_NAME}
	main.c
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_DIR}/api/cc/include/
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
