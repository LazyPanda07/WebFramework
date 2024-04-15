cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
if (UNIX)
	add_definitions(-D__LINUX__)

	set(CMAKE_CXX_FLAGS -rdynamic)
endif (UNIX)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
	executors/HelloExecutor.cpp
)

target_link_directories(
	${PROJECT_NAME} PUBLIC
	${WEB_FRAMEWORK_LIBRARY_PATH}/lib/
)

target_link_libraries(
	${PROJECT_NAME} PUBLIC
	${LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)

message("Folder: ${WEB_FRAMEWORK_LIBRARY_PATH}/lib/")
