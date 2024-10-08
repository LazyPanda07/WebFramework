cmake_minimum_required(VERSION 3.27.0)

project(csharp_test)

add_custom_target(
	${PROJECT_NAME} ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND dotnet publish csharp_test.csproj --output ${CMAKE_BINARY_DIR}/bin /p:DebugType=None /p:DebugSymbols=false
)

install(FILES run_csharp_test.bat DESTINATION bin)
install(FILES run_csharp_test.sh DESTINATION bin)
