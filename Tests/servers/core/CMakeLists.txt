cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
