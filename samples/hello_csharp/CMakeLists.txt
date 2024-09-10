cmake_minimum_required(VERSION 3.27.0)

project(hello_csharp)

add_custom_target(
	${PROJECT_NAME} ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND dotnet publish hello_csharp.csproj -o ${CMAKE_INSTALL_PREFIX} -p:DebugType=None -p:DebugSymbols=false -p:Configuration=Release -p:Platform=AnyCPU
)
