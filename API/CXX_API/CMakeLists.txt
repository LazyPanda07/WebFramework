cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CXX_API)

add_library(${PROJECT_NAME} INTERFACE)

if (UNIX)
	target_compile_definitions(
		${PROJECT_NAME} INTERFACE
		__LINUX__
	)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		target_compile_definitions(
			${PROJECT_NAME} INTERFACE
			__ANDROID__
		)
	endif()
endif()

target_include_directories(
	${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:api/cxx/include>
)

if (NOT ${AS_DEPENDENCY})
	install(TARGETS ${PROJECT_NAME} DESTINATION . EXPORT ${WEB_FRAMEWORK_EXPORT_TARGETS})
	install(DIRECTORY include DESTINATION api/cxx)
	install(
		EXPORT ${WEB_FRAMEWORK_EXPORT_TARGETS}
		FILE ${WEB_FRAMEWORK_EXPORT_TARGETS_FILE}
		NAMESPACE ${WEB_FRAMEWORK_EXPORT_TARGETS_NAMESPACE}
		DESTINATION cmake
	)
endif()
