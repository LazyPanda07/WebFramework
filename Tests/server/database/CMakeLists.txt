cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(Database)

add_library(
	${PROJECT_NAME} SHARED
	CRUDExecutor.cpp
	MultiUserExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
