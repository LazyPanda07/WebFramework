cmake_minimum_required(VERSION 3.27.0)

project(hello_executor)

add_library(
	${PROJECT_NAME} SHARED
	src/hello_executor.c
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES web.json DESTINATION .)
