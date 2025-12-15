cmake_minimum_required(VERSION 3.27.0)

project(csharp_hello_executor)

add_custom_target(
	${PROJECT_NAME} ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND dotnet publish csharp_hello_executor.csproj -o ${CMAKE_INSTALL_PREFIX}/executors /p:DebugType=None /p:DebugSymbols=false -p:Configuration=Release -p:Platform=AnyCPU
)

install(DIRECTORY settings/ DESTINATION executors)
