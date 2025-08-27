cmake_minimum_required(VERSION 3.27.0)

project(DatabaseExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/CRUDExecutor.c
	src/MultiUserExecutor.c
	src/SQLUtility.c
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include/
	${WEB_FRAMEWORK_INCLUDE}
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
