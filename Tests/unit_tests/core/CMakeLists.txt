cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(REPROC_TAG v14.2.5)

FetchContent_Declare(
	reproc++
	GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc.git
	GIT_TAG ${REPROC_TAG}
)

set(REPROC++ ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(reproc++)

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
	reproc++
)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Localization>
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)

add_test(NAME ${PROJECT_NAME} COMMAND Core --server_config multi_threaded_config.json WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
