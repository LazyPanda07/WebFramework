cmake_minimum_required(VERSION 3.27.0)

project(cxx_test)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin)

if (UNIX)
	list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../WebFrameworkLibrary)

find_package(WebFramework REQUIRED)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES run_cxx_test.sh DESTINATION .)
