cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_CC_API ON)

project(hello_c)

include(${WEB_FRAMEWORK_DIRECTORY}/WebFrameworkConfig.cmake)

add_executable(
	${PROJECT_NAME}
	main.c
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
