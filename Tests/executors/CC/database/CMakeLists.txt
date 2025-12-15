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
	include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
