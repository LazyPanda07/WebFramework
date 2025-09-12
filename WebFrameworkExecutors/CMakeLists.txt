cmake_minimum_required(VERSION 3.27.0)

project(WebFrameworkExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/Executors/BaseExecutor.cpp
	src/Executors/BaseHeavyOperationStatefulExecutor.cpp
	src/Executors/BaseHeavyOperationStatelessExecutor.cpp
	src/Executors/BaseStatefulExecutor.cpp
	src/Executors/BaseStatelessExecutor.cpp
	src/Utility/JSONSettingsParser.cpp
	src/Exceptions/BaseExecutorException.cpp
	src/Exceptions/BadRequestException.cpp
	src/Exceptions/NotImplementedException.cpp
	src/c_api/executors_c_api.cpp
	src/Web/HTTPRequestExecutors.cpp
	src/Web/HTTPResponseExecutors.cpp
	src/Databases/DatabaseExecutors.cpp
	src/Databases/TableExecutors.cpp
	src/Databases/SQLValueExecutors.cpp
	src/Databases/SQLResultExecutors.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	src/
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
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)
