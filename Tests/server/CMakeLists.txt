cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(LOCALIZATION_BUILD_TYPE build_release_localization)
set(DEBUG_LOCALIZATION_DIR ${CMAKE_INSTALL_PREFIX})
set(RELEASE_LOCALIZATION_DIR ${CMAKE_INSTALL_PREFIX})

if(UNIX)
	file(CHMOD ${WEB_FRAMEWORK_LIBRARY_DIR}/assets/LocalizationUtils PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
endif(UNIX)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
)

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${WEB_FRAMEWORK_LIBRARY_DIR}/localization_modules.json ${CMAKE_INSTALL_PREFIX}/localization_modules.json
)

if(NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_MAKE_PROGRAM} ${LOCALIZATION_BUILD_TYPE}
	)
endif()

add_subdirectory(database)
add_subdirectory(server_executors)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
