cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(SQLITE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/WebFramework/vendor/sqlite3/lib)
set(SQLITE_DLL_PATH ${CMAKE_SOURCE_DIR}/WebFramework/vendor/sqlite3)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/WebFrameworkLibrary CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(WEB_FRAMEWORK_LIBRARY_TYPE STATIC)
set(
	LIBS
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
	sqlite3
	ssl
	crypto
)
option(SHARED_WEB_FRAMEWORK "" ON)

project(WebFramework)

if (UNIX)
	add_definitions(-D__LINUX__)
	add_compile_options(-rdynamic)
	add_compile_options(-fvisibility=hidden)

	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Linux)
	set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Linux/libsqlite3.so)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/lib/vendor/sqlite3)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		add_definitions(-D__ANDROID__)

		set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Android)
		set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/lib/Android/libsqlite3.so)
		
		install(DIRECTORY WebFramework/vendor/sqlite3/lib/Android/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	else()
		install(DIRECTORY WebFramework/vendor/sqlite3/lib/Linux/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	endif()
else ()
	add_definitions(-DNOMINMAX)
	add_definitions(-D_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)

	set(SQLITE_LIBRARY_PATH ${SQLITE_LIBRARY_PATH}/Windows)
	set(SQLITE_DLL_PATH ${SQLITE_DLL_PATH}/dll/sqlite3.dll)
	set(SQLITE_DLL_DESTINATION_PATH ${CMAKE_INSTALL_PREFIX}/dll/vendor/sqlite3)
	set(LIBS ${LIBS} crypt32)

	install(DIRECTORY WebFramework/vendor/sqlite3/lib/Windows/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif ()

if (${SHARED_WEB_FRAMEWORK})
	add_definitions(-DWEB_FRAMEWORK_DLL)

	set(WEB_FRAMEWORK_LIBRARY_TYPE SHARED)
endif ()

add_library(
	${PROJECT_NAME} ${WEB_FRAMEWORK_LIBRARY_TYPE}
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
	WebFramework/src/Executors/BaseExecutor.cpp
	WebFramework/src/Executors/BaseHeavyOperationStatefulExecutor.cpp
	WebFramework/src/Executors/BaseHeavyOperationStatelessExecutor.cpp
	WebFramework/src/Executors/BaseStatefulExecutor.cpp
	WebFramework/src/Executors/BaseStatelessExecutor.cpp
	WebFramework/src/Executors/ResourceExecutor.cpp
	WebFramework/src/LoadBalancer/LoadBalancerServer.cpp
	WebFramework/src/LoadBalancer/Heuristics/BaseLoadBalancerHeuristic.cpp
	WebFramework/src/LoadBalancer/Heuristics/Connections.cpp
	WebFramework/src/Proxy/ProxyServer.cpp
	WebFramework/src/SQLite3/SQLiteDatabase.cpp
	WebFramework/src/SQLite3/SQLiteDatabaseModel.cpp
	WebFramework/src/SQLite3/SQLiteResult.cpp
	WebFramework/src/SQLite3/SQLiteManager.cpp
	WebFramework/src/Utility/Sources.cpp
	WebFramework/src/Utility/RouteParameters.cpp
	WebFramework/src/Utility/JSONSettingsParser.cpp
	WebFramework/src/Utility/Singletons/HTTPSSingleton.cpp
	WebFramework/src/Utility/BaseConnectionData.cpp
	WebFramework/src/Utility/DynamicLibraries.cpp
	WebFramework/src/Managers/ExecutorsManager.cpp
	WebFramework/src/Managers/SessionsManager.cpp
	WebFramework/src/WebNetwork/HTTPRequest.cpp
	WebFramework/src/WebNetwork/HTTPResponse.cpp
	WebFramework/src/WebNetwork/WebServers/BaseWebServer.cpp
	WebFramework/src/WebNetwork/WebServers/MultithreadedWebServer.cpp
	WebFramework/src/WebNetwork/WebServers/ThreadPoolWebServer.cpp
	WebFramework/src/WebNetwork/Interfaces/IExecutorFunctionality.cpp
	WebFramework/src/Import/Config.cpp
	WebFramework/src/Import/WebFramework.cpp
	WebFramework/src/c_api/web_framework_c_api.cpp
)

if (WIN32)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif (WIN32)

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

add_subdirectory(API)

target_link_directories(
	${PROJECT_NAME} PUBLIC
	${SQLITE_LIBRARY_PATH}
)

message("SQLite path: ${SQLITE_LIBRARY_PATH}")

target_link_libraries(
	${PROJECT_NAME}
	${LIBS}
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)

install(FILES ${SQLITE_DLL_PATH} DESTINATION ${SQLITE_DLL_DESTINATION_PATH})
install(FILES WebFramework.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})

if (NOT EXISTS ${CMAKE_INSTALL_PREFIX}/WebFrameworkAssets)
	install(DIRECTORY WebFramework/WebFrameworkAssets/ DESTINATION ${CMAKE_INSTALL_PREFIX}/WebFrameworkAssets)
endif ()

install(DIRECTORY WebFramework/src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY WebFramework/vendor/sqlite3/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/vendor/sqlite3/)
install(DIRECTORY WebFramework/certificates DESTINATION ${CMAKE_INSTALL_PREFIX})
