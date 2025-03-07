cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
	add_definitions(-D__LINUX__)

	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -rdynamic)
endif(UNIX)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

project(DefaultHTTPSServer)

add_executable(
	${PROJECT_NAME}
	main.cpp
	Executor.cpp
	RoutePatternExecutor.cpp
	UserAgentExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
