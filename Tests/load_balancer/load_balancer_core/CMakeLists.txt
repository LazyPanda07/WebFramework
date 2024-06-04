cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(LoadBalancerCore)

add_executable(
	${PROJECT_NAME}
	main.cpp
	utilities/utilities.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	utilities/
	${WEB_FRAMEWORK_LIBRARY_DIR}/include/
	${WEB_FRAMEWORK_LIBRARY_DIR}/include/vendor/OpenSSL/
	${GTEST_INCLUDE_DIRS}
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_LIB_DIRS}
	${GTEST_LIB_DIR}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${LIBS}
	${GTEST_BOTH_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
