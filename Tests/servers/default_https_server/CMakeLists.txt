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

install(TARGETS ${PROJECT_NAME} DESTINATION .)
