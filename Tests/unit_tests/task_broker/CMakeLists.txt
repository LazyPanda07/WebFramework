cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(TaskBrokerTests)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	GTest::gtest
	GTest::gtest_main
	unit_test_utils
	Networks
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/scripts/run_task_broker_tests.bat DESTINATION .)
install(FILES ${CMAKE_SOURCE_DIR}/scripts/run_task_broker_tests.sh DESTINATION .)
