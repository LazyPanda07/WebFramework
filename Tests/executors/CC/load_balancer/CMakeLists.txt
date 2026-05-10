cmake_minimum_required(VERSION 3.27.0)

project(LoadBalancerExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/IdExecutor.c
	src/CustomHeuristic.c
)

if (UNIX)
	find_package(Threads REQUIRED)

	target_link_libraries(
		${PROJECT_NAME} PRIVATE
		Threads::Threads
	)
endif()

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
