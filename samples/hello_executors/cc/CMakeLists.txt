cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_CC_API ON)

project(cc_hello_executors)

include(${WEB_FRAMEWORK_DIRECTORY}/WebFrameworkConfig.cmake)

add_executable(
	${PROJECT_NAME}
	src/hello_executor.c
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION executors)
install(DIRECTORY settings/ DESTINATION executors)
