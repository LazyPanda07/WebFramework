cmake_minimum_required(VERSION 3.27.0)

project(CSharpAPI)

if (UNIX)
	add_custom_target(
		${PROJECT_NAME} ALL
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Tests
		COMMAND dotnet publish Tests.sln -o ${CMAKE_INSTALL_PREFIX} /p:Configuration=Release /p:Plaftorm='Any CPU' /p:DebugType=None /p:DebugSymbols=false
	)
else()
	add_custom_target(
		${PROJECT_NAME} ALL
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Tests
		COMMAND dotnet publish Tests.sln -o ${CMAKE_INSTALL_PREFIX} /p:Configuration=Release /p:Plaftorm="Any CPU" /p:DebugType=None /p:DebugSymbols=false
	)
endif()
