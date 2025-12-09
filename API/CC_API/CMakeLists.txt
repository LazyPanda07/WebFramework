cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_C_STANDARD 11)

if (UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_API)

add_library(
	${PROJECT_NAME} STATIC
	src/Executors/Executor.c
	src/Databases/Database.c
	src/Databases/SQLResult.c
	src/Databases/SQLValue.c
	src/Databases/Table.c
	src/Utility/WebFrameworkUtility.c
	src/Utility/WebFrameworkLocalization.c
	src/Config.c
	src/DLLHandler.c
	src/HttpRequest.c
	src/HttpResponse.c
	src/JsonBuilder.c
	src/JsonObject.c
	src/JsonParser.c
	src/WebFramework.c
	src/WebFrameworkException.c
	src/LoadBalancerHeuristic.c
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:api/cc/include>
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib EXPORT ${WEB_FRAMEWORK_EXPORT_TARGETS})
install(DIRECTORY include DESTINATION api/cc)
install(
	EXPORT ${WEB_FRAMEWORK_EXPORT_TARGETS}
	FILE ${WEB_FRAMEWORK_EXPORT_TARGETS_FILE}
	NAMESPACE ${WEB_FRAMEWORK_EXPORT_TARGETS_NAMESPACE}
	DESTINATION cmake
)
