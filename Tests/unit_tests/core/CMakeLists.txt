cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
option(WITHOUT_REDIS_TESTS "" OFF)

project(Core)

if (${WITHOUT_REDIS_TESTS})
	add_definitions(-DWITHOUT_REDIS_TESTS)
endif()

add_executable(
	${PROJECT_NAME}
	main.cpp
	utilities/utilities.cpp
	cases/Requests.cpp
	cases/Database.cpp
	cases/MultiUserDatabase.cpp
	cases/WFDP.cpp
	cases/ResourceLoading.cpp
	cases/Chunks.cpp
	cases/Localization.cpp
	cases/UserAgentFilter.cpp
	cases/ResourceUploading.cpp
	cases/Redis.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	utilities
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	Localization
	Networks
	UtilityLibrary
	-Wl,--whole-archive
	JSON
	-Wl,--no-whole-archive
	HTTP
	SocketStreams
	GTest::gtest
	GTest::gtest_main
	${3RDPARTY_LIBRARIES}
	WebFramework::CXX_API
	unit_test_utils
)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Localization>
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)

add_test(NAME ${PROJECT_NAME} COMMAND Core.exe --server_config multi_threaded_redis_config.json --run_redis_tests --run_arguments Server.exe WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
