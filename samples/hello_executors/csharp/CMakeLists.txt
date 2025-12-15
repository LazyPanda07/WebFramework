cmake_minimum_required(VERSION 3.27.0)

project(csharp_hello_executor)

add_custom_target(
	${PROJECT_NAME} ALL
	COMMAND dotnet publish -o ${CMAKE_INSTALL_PREFIX}/executors /p:DebugType=None /p:DebugSymbols=false
)
