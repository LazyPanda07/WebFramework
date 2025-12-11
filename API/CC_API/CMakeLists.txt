cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_C_STANDARD 11)

if (UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_API)

add_library(
	${PROJECT_NAME} STATIC
	src/executors/executor.c
	src/databases/database.c
	src/databases/sql_result.c
	src/databases/sql_value.c
	src/databases/table.c
	src/utility/web_framework_utility.c
	src/utility/web_framework_localization.c
	src/config.c
	src/dll_handler.c
	src/http_request_t.c
	src/http_response.c
	src/json_builder.c
	src/json_object.c
	src/json_parser.c
	src/web_framework.c
	src/web_framework_exception.c
	src/load_balancer_heuristic.c
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
