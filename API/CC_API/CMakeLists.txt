cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_C_STANDARD 11)

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

project(CC_API)

add_library(
	${PROJECT_NAME} STATIC
	src/Executors/BaseExecutor.c
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
	include
)

install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION dll
)

install(DIRECTORY include DESTINATION api/cc)
