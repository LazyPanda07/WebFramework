cmake_minimum_required(VERSION 3.27.0)

project(CXX_API)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${GTEST_INCLUDE_DIRS}
)

target_link_directories(
	${PROJECT_NAME} PRIVATE
	${GTEST_LIB_DIR}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${GTEST_BOTH_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
