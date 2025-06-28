cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/WebFrameworkLibrary CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(
	LIBS
	WebFrameworkCore
	WebFrameworkExecutors
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
	Databases
	sqlite3
	ssl
	crypto
)
option(WITH_LANGUAGES_APIS "Build all languages APIS" ON)

project(WebFramework VERSION 3.0.12)

if (UNIX)
	add_definitions(-D__LINUX__)
	add_compile_options(-fvisibility=hidden)
	
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		add_definitions(-D__ANDROID__)
	else()
		add_compile_options(-rdynamic)
	endif()
else ()
	add_definitions(-DNOMINMAX)

	set(LIBS ${LIBS} crypt32)
endif ()

add_library(
	${PROJECT_NAME} SHARED
	WebFramework/src/DynamicPages/StandardWebFrameworkDynamicPagesFunctions.cpp
	WebFramework/src/DynamicPages/WebFrameworkDynamicPages.cpp
	WebFramework/src/Exceptions/NotFoundException.cpp
	WebFramework/src/Exceptions/CantFindFunctionException.cpp
	WebFramework/src/Exceptions/CantLoadSourceException.cpp
	WebFramework/src/Exceptions/DynamicPagesSyntaxException.cpp
	WebFramework/src/Exceptions/MissingLoadTypeException.cpp
	WebFramework/src/Exceptions/APIException.cpp
	WebFramework/src/Executors/ResourceExecutor.cpp
	WebFramework/src/LoadBalancer/LoadBalancerServer.cpp
	WebFramework/src/LoadBalancer/Heuristics/BaseLoadBalancerHeuristic.cpp
	WebFramework/src/LoadBalancer/Heuristics/Connections.cpp
	WebFramework/src/Proxy/ProxyServer.cpp
	WebFramework/src/Utility/Sources.cpp
	WebFramework/src/Utility/RouteParameters.cpp
	WebFramework/src/Utility/Singletons/HTTPSSingleton.cpp
	WebFramework/src/Utility/BaseConnectionData.cpp
	WebFramework/src/Utility/DynamicLibraries.cpp
	WebFramework/src/Managers/DatabasesManager.cpp
	WebFramework/src/Managers/ExecutorsManager.cpp
	WebFramework/src/WebNetwork/WebServers/BaseWebServer.cpp
	WebFramework/src/WebNetwork/WebServers/MultithreadedWebServer.cpp
	WebFramework/src/WebNetwork/WebServers/ThreadPoolWebServer.cpp
	WebFramework/src/WebNetwork/Interfaces/IExecutorFunctionality.cpp
	WebFramework/src/Framework/Config.cpp
	WebFramework/src/Framework/WebFramework.cpp
	WebFramework/src/CAPI/WebFrameworkCAPI.cpp
	WebFramework/src/CAPI/WebFrameworkExecutorsCAPI.cpp
	WebFramework/src/CAPI/WebFrameworkUtilityCAPI.cpp
	WebFramework/src/Utility/LargeFileHandlers/BaseLargeBodyHandler.cpp
	WebFramework/src/Utility/LargeFileHandlers/ThreadPoolHandler.cpp
	WebFramework/src/Utility/LargeFileHandlers/MultithreadedHandler.cpp
	WebFramework/src/Utility/AdditionalServerSettings.cpp
	WebFramework/src/WebNetwork/HTTPRequestImplementation.cpp
	WebFramework/src/WebNetwork/HTTPResponseImplementation.cpp
	WebFramework/src/Databases/DatabaseImplementation.cpp
	WebFramework/src/Databases/TableImplementation.cpp
	WebFramework/src/Databases/SQLResultImplementation.cpp
	WebFramework/src/Databases/SQLValueImplementation.cpp
	WebFramework/src/Executors/CXXExecutor.cpp
)

if (DEFINED ENV{MARCH} AND NOT "$ENV{MARCH}" STREQUAL "")
	target_compile_options(${PROJECT_NAME} PRIVATE -march=$ENV{MARCH})
endif()

if (CMAKE_GENERATOR MATCHES "Visual Studio")
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

target_include_directories(
	${PROJECT_NAME} PUBLIC
	WebFramework/src/
	WebFrameworkCore/src/
	WebFrameworkCore/vendor/sqlite3/include/
	WebFrameworkExecutors/src/
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

add_subdirectory(WebFrameworkCore)
add_subdirectory(WebFrameworkExecutors)
add_subdirectory(BaseTCPServer)
add_subdirectory(FileManager)
add_subdirectory(INIParser)
add_subdirectory(Localization)
add_subdirectory(Log)
add_subdirectory(Networks)
add_subdirectory(SHA256)
add_subdirectory(ThreadPool)
add_subdirectory(UtilityLibrary)
add_subdirectory(Databases)

add_subdirectory(API)

target_link_directories(
	${PROJECT_NAME} PUBLIC
	${SQLITE_LIBRARY_PATH}
)

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

install(FILES WebFrameworkConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES FindWebFramework.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})

if (NOT EXISTS ${CMAKE_INSTALL_PREFIX}/WebFrameworkAssets)
	install(DIRECTORY WebFramework/WebFrameworkAssets/ DESTINATION ${CMAKE_INSTALL_PREFIX}/WebFrameworkAssets)
endif ()

install(DIRECTORY WebFramework/src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY WebFramework/certificates DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY schemas DESTINATION ${CMAKE_INSTALL_PREFIX})
