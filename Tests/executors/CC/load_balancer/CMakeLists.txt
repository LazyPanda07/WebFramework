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

install(TARGETS ${PROJECT_NAME} DESTINATION .)
