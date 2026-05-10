cmake_minimum_required(VERSION 3.27.0)

project(LoadBalancerExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/IdExecutor.cpp
	src/CustomHeuristic.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    WebFramework::CXX_API
)

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
