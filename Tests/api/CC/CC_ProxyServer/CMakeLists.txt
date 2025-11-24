cmake_minimum_required(VERSION 3.27.0)

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_ProxyServer)

add_executable(
	${PROJECT_NAME}
	main.c
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
