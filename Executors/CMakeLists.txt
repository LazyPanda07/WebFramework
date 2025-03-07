cmake_minimum_required(VERSION 3.27.0)

project(Executors)

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
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	src/
)

target_link_libraries(
	${PROJECT_NAME}
	WebFrameworkCore
	JSON
	FileManager
	HTTP
	UtilityLibrary
	SocketStreams
	Networks
	Log
	Localization
	BaseTCPServer
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
