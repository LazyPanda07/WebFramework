cmake_minimum_required(VERSION 3.27.0)

project(DatabaseExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/CRUDExecutor.c
	src/MultiUserExecutor.c
	src/SQLUtility.c
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
