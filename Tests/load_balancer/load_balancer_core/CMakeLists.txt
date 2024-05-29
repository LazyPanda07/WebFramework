cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
	add_definitions(-D__LINUX__)
endif (UNIX)

project(LoadBalancerCore)

add_executable(
	${PROJECT_NAME}
	main.cpp
	utilities/utilities.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	utilities/
	${WEB_FRAMEWORK_DIR}/Networks/src/
	${WEB_FRAMEWORK_DIR}/Networks/HTTP/src/
	${WEB_FRAMEWORK_DIR}/Networks/HTTP/JSON/src/
	${WEB_FRAMEWORK_DIR}/Networks/SocketStreams/src/
	${WEB_FRAMEWORK_DIR}/Networks/vendor/OpenSSL/include/
	${WEB_FRAMEWORK_DIR}/UtilityLibrary/include/
	${GTEST_INCLUDE_DIR}
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBRARY_DIR}/lib/
	${GTEST_LIB_DIR}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${LIBS}
	gtest
	gtest_main
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)