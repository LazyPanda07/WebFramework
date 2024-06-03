cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(WebFrameworkSDK ${WEB_FRAMEWORK_LIBRARY_DIR}/)
set(LOCALIZATION_BUILD_TYPE build_release_localization)
set(DEBUG_LOCALIZATION_DIR ${CMAKE_INSTALL_PREFIX})
set(RELEASE_LOCALIZATION_DIR ${CMAKE_INSTALL_PREFIX})

if(UNIX)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -rdynamic)

	file(CHMOD ${WEB_FRAMEWORK_LIBRARY_DIR}/assets/LocalizationUtils PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
endif(UNIX)

if(WIN32)
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(LOCALIZATION_BUILD_TYPE build_debug_localization)
	endif()
endif(WIN32)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFramework.cmake)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
	TextGenerator.cpp
	executors/HelloExecutor.cpp
	executors/AssetsExecutor.cpp
	executors/ChunksExecutor.cpp
	executors/DownloadExecutor.cpp
	executors/LocalizationExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${WEB_FRAMEWORK_LIBRARY_DIR}/localization_modules.json ${CMAKE_INSTALL_PREFIX}/localization_modules.json
)

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_MAKE_PROGRAM} ${LOCALIZATION_BUILD_TYPE}
)

add_subdirectory(database)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
