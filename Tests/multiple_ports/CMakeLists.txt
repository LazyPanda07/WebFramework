cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)

project(MultiplePortsServer)

add_executable(
	${PROJECT_NAME}
	main.cpp
	src/MultiplePorts.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

if (UNIX)
	target_link_options(
		${PROJECT_NAME} PRIVATE
		-rdynamic
	)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES multiple_ports_config.json DESTINATION .)
install(FILES multiple_ports_web.json DESTINATION .)
