cmake_minimum_required(VERSION 3.27.0)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFramework.cmake)

project(CC_API)

add_executable(
	${PROJECT_NAME}
	main.c
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
	${WEB_FRAMEWORK_LIBS}
	${GTEST_BOTH_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
