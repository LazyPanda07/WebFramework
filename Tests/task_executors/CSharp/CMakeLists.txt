cmake_minimum_required(VERSION 3.27.0)

project(csharp_test_task_executors)

add_custom_target(
	${PROJECT_NAME} ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND dotnet publish -o ${CMAKE_INSTALL_PREFIX} -c Release /p:DebugType=None /p:DebugSymbols=false
)
