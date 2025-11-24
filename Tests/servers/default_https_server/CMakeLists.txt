cmake_minimum_required(VERSION 3.27.0)

if(UNIX)
	list(APPEND CMAKE_CXX_FLAGS -rdynamic)
endif(UNIX)

project(DefaultHTTPSServer)

add_executable(
	${PROJECT_NAME}
	main.cpp
	Executor.cpp
	RoutePatternExecutor.cpp
	UserAgentExecutor.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
