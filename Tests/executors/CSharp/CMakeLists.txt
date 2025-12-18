cmake_minimum_required(VERSION 3.27.0)

project(csharp_executors)

add_custom_target(
	build_core_executors ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/CoreExecutors
	COMMAND dotnet publish -o ${CMAKE_BINARY_DIR}/bin /p:DebugType=None /p:DebugSymbols=false
)

add_custom_target(
	build_database_executors ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/DatabaseExecutors
	COMMAND dotnet publish -o ${CMAKE_BINARY_DIR}/bin /p:DebugType=None /p:DebugSymbols=false
)

add_custom_target(
	build_load_balancer_executors ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/LoadBalancerExecutors
	COMMAND dotnet publish -o ${CMAKE_BINARY_DIR}/bin /p:DebugType=None /p:DebugSymbols=false
)
