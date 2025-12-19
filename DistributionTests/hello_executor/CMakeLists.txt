cmake_minimum_required(VERSION 3.27.0)

project(hello_executor)

if (UNIX)
	list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)

find_package(WebFramework REQUIRED)

add_library(
	${PROJECT_NAME} SHARED
	src/HelloExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES web.json DESTINATION .)
