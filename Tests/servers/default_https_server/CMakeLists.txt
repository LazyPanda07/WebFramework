cmake_minimum_required(VERSION 3.27.0)

project(DefaultHTTPSServer)

add_executable(
	${PROJECT_NAME}
	main.cpp
	Executor.cpp
	RoutePatternExecutor.cpp
	UserAgentExecutor.cpp
)

if (UNIX)
	target_link_options(
		${PROJECT_NAME} PRIVATE
		-rdynamic
	)
endif()

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
