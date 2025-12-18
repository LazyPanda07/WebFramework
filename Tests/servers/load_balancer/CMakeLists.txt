cmake_minimum_required(VERSION 3.27.0)

project(LoadBalancerServer)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	UtilityLibrary
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
