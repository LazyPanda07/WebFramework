cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(Core)

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
	cases/Token.cpp
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

if (${CC_API})
	add_test(NAME CC_API_TEST COMMAND CC_API_TESTS WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
	add_test(NAME ${PROJECT_NAME} COMMAND Core --server_config multi_threaded_config.json --run_arguments CC_Server WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
elseif(${CXX_API})
	add_test(NAME CXX_API_TEST COMMAND CXX_API_TESTS WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
	add_test(NAME ${PROJECT_NAME} COMMAND Core --server_config multi_threaded_config.json --run_arguments Server WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
elseif (${PYTHON_API})
	if (UNIX)
		add_test(NAME PYTHON_API_TEST COMMAND python3 api_test.py WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
		add_test(NAME ${PROJECT_NAME} COMMAND Core --server_config multi_threaded_config.json --run_arguments "python3 -u server.py" WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
	else()
		add_test(NAME PYTHON_API_TEST COMMAND python api_test.py WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
		add_test(NAME ${PROJECT_NAME} COMMAND Core --server_config multi_threaded_config.json --run_arguments "python -u server.py" WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
	endif()
elseif (${CSHARP_API})
	add_test(NAME CSHARP_API_TEST COMMAND dotnet test CSharpAPI.dll WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
	add_test(NAME ${PROJECT_NAME} COMMAND Core --server_config multi_threaded_config.json --run_arguments "dotnet CSharpServer.dll" WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
endif()
