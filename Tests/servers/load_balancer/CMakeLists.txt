cmake_minimum_required(VERSION 3.27.0)

project(LoadBalancerServer)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_INCLUDE}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	UtilityLibrary
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
