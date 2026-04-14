cmake_minimum_required(VERSION 3.27.0)

project(WebFrameworkExecutors)

add_library(
	${PROJECT_NAME} STATIC
	src/Executors/Executor.cpp
	src/Executors/HeavyOperationStatefulExecutor.cpp
	src/Executors/HeavyOperationStatelessExecutor.cpp
	src/Executors/StatefulExecutor.cpp
	src/Executors/StatelessExecutor.cpp
	src/Utility/JSONSettingsParser.cpp
	src/Exceptions/ExecutorException.cpp
	src/Exceptions/BadRequestException.cpp
	src/Exceptions/NotImplementedException.cpp
	src/Exceptions/NotFoundException.cpp
	src/Exceptions/ForbiddenException.cpp
)

if (${WITH_UNITY_BUILD})
	set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
endif()

target_include_directories(
	${PROJECT_NAME} PUBLIC
	include/
)

target_link_libraries(
	${PROJECT_NAME}
	FileManager
	HTTP
	JSON
	Localization
	Log
	UtilityLibrary
	WebFrameworkCore
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION dll
)
