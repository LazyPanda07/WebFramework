cmake_minimum_required(VERSION 3.27.0)

project(LoadBalancerExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/IdExecutor.cpp
	src/CustomHeuristic.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include/
	${WEB_FRAMEWORK_INCLUDE}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIB}
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
