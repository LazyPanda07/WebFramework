cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(SQLITE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/WebFramework/vendor/sqlite3/lib)
set(SQLITE_DLL_PATH ${CMAKE_SOURCE_DIR}/WebFramework/vendor/sqlite3)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install-dir CACHE STRING "")
option(BUILD_SHARED_LIBS "" OFF)

if (UNIX)
	add_definitions(-D__LINUX__)

	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Linux)
	set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Linux/libsqlite3.so)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/lib)
else ()
	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Windows)
	set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/dll/sqlite3.dll)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/dll)
endif ()

if (${BUILD_SHARED_LIBS})
	add_definitions(-DWEB_FRAMEWORK_DLL)
endif ()

project(WebFramework)

add_library(
	${PROJECT_NAME}
	WebFramework/src/DynamicPages/StandardWebFrameworkDynamicPagesFunctions.cpp
	WebFramework/src/DynamicPages/WebFrameworkDynamicPages.cpp
	WebFramework/src/Exceptions/BadRequestException.cpp
	WebFramework/src/Exceptions/BaseExecutorException.cpp
	WebFramework/src/Exceptions/BaseWebFrameworkException.cpp
	WebFramework/src/Exceptions/CantFindFunctionException.cpp
	WebFramework/src/Exceptions/CantLoadSourceException.cpp
	WebFramework/src/Exceptions/DatabaseException.cpp
	WebFramework/src/Exceptions/DynamicPagesSyntaxException.cpp
	WebFramework/src/Exceptions/MissingLoadTypeException.cpp
	WebFramework/src/Exceptions/NotImplementedException.cpp
	WebFramework/src/Exceptions/SQLite3Exception.cpp
	WebFramework/src/Exceptions/StringConversionException.cpp
	WebFramework/src/Executors/BaseExecutor.cpp
	WebFramework/src/Executors/BaseHeavyOperationStatefulExecutor.cpp
	WebFramework/src/Executors/BaseHeavyOperationStatelessExecutor.cpp
	WebFramework/src/Executors/BaseStatefulExecutor.cpp
	WebFramework/src/Executors/BaseStatelessExecutor.cpp
	WebFramework/src/Executors/ResourceExecutor.cpp
	WebFramework/src/LoadBalancer/LoadBalancer.cpp
	WebFramework/src/LoadBalancer/LoadBalancerWebNetwork/LoadBalancerServer.cpp
	WebFramework/src/Middleware/BaseMiddlewareRouteController.cpp
	WebFramework/src/Middleware/Middleware.cpp
	WebFramework/src/Middleware/MiddlewareWebNetwork/MiddlewareServer.cpp
	WebFramework/src/SQLite3/SQLiteDatabase.cpp
	WebFramework/src/SQLite3/SQLiteDatabaseModel.cpp
	WebFramework/src/SQLite3/SQLiteResult.cpp
	WebFramework/src/Utility/StringConversion.cpp
	WebFramework/src/Utility/RouteParameters.cpp
	WebFramework/src/Utility/JSONSettingsParser.cpp
	WebFramework/src/Utility/ExecutorCreator.cpp
	WebFramework/src/Utility/CharactersEncoding.cpp
	WebFramework/src/Utility/Singletons/HTTPSSingleton.cpp
	WebFramework/src/Utility/Middleware/BaseConnectionData.cpp
	WebFramework/src/WebFrameworkManagers/ExecutorsManager.cpp
	WebFramework/src/WebFrameworkManagers/SessionsManager.cpp
	WebFramework/src/WebNetwork/HTTPRequest.cpp
	WebFramework/src/WebNetwork/HTTPResponse.cpp
	WebFramework/src/WebNetwork/WebFrameworkHTTPNetwork.cpp
	WebFramework/src/WebNetwork/WebFrameworkHTTPSNetwork.cpp
	WebFramework/src/WebNetwork/WebServers/BaseWebServer.cpp
	WebFramework/src/WebNetwork/WebServers/MultithreadedWebServer.cpp
	WebFramework/src/WebNetwork/WebServers/ThreadPoolWebServer.cpp
	WebFramework/src/WebFramework.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	WebFramework/vendor/sqlite3/include/
	WebFramework/src/
	BaseTCPServer/src/
	FileManager/src/
	INIParser/src/
	Localization/src/
	Log/src/
	Networks/src/
	Networks/HTTP/src/
	Networks/HTTP/JSON/src/
	SHA256/src/
	ThreadPool/src/
	UtilityLibrary/include/
)

add_subdirectory(BaseTCPServer)
add_subdirectory(FileManager)
add_subdirectory(INIParser)
add_subdirectory(Localization)
add_subdirectory(Log)
add_subdirectory(Networks)
add_subdirectory(SHA256)
add_subdirectory(ThreadPool)
add_subdirectory(UtilityLibrary)

target_link_directories(
	${PROJECT_NAME}
	${SQLITE_LIBRARY_PATH}
)

target_link_libraries(
	${PROJECT_NAME}
	BaseTCPServer
	FileManager
	INIParser
	Localization
	Log
	Networks
	JSON
	SocketStreams
	SHA256
	ThreadPool
	UtilityLibrary
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)

install(FILES ${SQLITE_DLL_PATH} DESTINATION ${SQLITE_DLL_DESTINATION_PATH})

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/WebFramework/src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN ".h")
endif ()
