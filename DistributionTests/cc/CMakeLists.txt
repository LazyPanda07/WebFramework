cmake_minimum_required(VERSION 3.27.0)

project(cc_test)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

set(WEB_FRAMEWORK_CC_API TRUE)

list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)

find_package(WebFramework REQUIRED)

add_executable(
	${PROJECT_NAME}
	main.c
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES run_cc_test.sh DESTINATION ${CMAKE_INSTALL_PREFIX})
