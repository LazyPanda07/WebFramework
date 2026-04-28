cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WEB_FRAMEWORK_EXPORT_TARGETS WebFrameworkTargets)
set(WEB_FRAMEWORK_EXPORT_TARGETS_FILE WebFrameworkTargets.cmake)
set(WEB_FRAMEWORK_EXPORT_TARGETS_NAMESPACE WebFramework::)
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(FETCHCONTENT_QUIET OFF CACHE BOOL "")
set(
	3RDPARTY_LIBS
	sqlite3
	hiredis
	rabbitmq::rabbitmq-static
	jwt-cpp::jwt-cpp
	ssl
	crypto
	CACHE INTERNAL ""
)
set(ADDITIONAL_LIBS "" CACHE INTERNAL "")
set(ADDITIONAL_INCLUDES "" CACHE INTERNAL "")
set(ADDITIONAL_LIB_DIRS "" CACHE INTERNAL "")
set(CXX_API_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/api/cxx/include)
set(PYTHON_EXECUTABLE "" CACHE STRING "Path to Python executable")
option(WITH_LANGUAGES_APIS "Build all languages APIS" ON)
option(WITH_STACKTRACE "Build with Boost::stacktrace" OFF)
option(WITH_PYTHON_EXECUTORS "Build with Python Executors" ON)
option(WITH_DOTNET_EXECUTORS "Build with .NET Executors" ON)
option(WITH_UNITY_BUILD "Build few .cpp at once" ON)
option(AS_DEPENDENCY "Change install paths for FetchContent or add_subdirectory usage" OFF)

project(WebFramework VERSION 3.3.7)

set(SHARED_LIBRARY_PROJECT_NAME ${PROJECT_NAME})

install(FILES LICENSE DESTINATION license) # Used in APIs install

add_library(
	${PROJECT_NAME} SHARED
	WebFramework/src/WFDP/StandardWebFrameworkDynamicPagesFunctions.cpp
	WebFramework/src/WFDP/CXXDynamicFunction.cpp
	WebFramework/src/WFDP/CCDynamicFunction.cpp
	WebFramework/src/WFDP/PythonDynamicFunction.cpp
	WebFramework/src/WFDP/CSharpDynamicFunction.cpp
	WebFramework/src/Rendering/WFDPRenderer.cpp
	WebFramework/src/Rendering/MDRenderer.cpp
	WebFramework/src/Runtimes/Runtime.cpp
	WebFramework/src/Runtimes/CXXRuntime.cpp
	WebFramework/src/Runtimes/CCRuntime.cpp
	WebFramework/src/Runtimes/PythonRuntime.cpp
	WebFramework/src/Runtimes/DotNetRuntime.cpp
	WebFramework/src/Exceptions/CantFindFunctionException.cpp
	WebFramework/src/Exceptions/CantLoadSourceException.cpp
	WebFramework/src/Exceptions/DynamicPagesSyntaxException.cpp
	WebFramework/src/Exceptions/MissingLoadTypeException.cpp
	WebFramework/src/Exceptions/APIException.cpp
	WebFramework/src/Exceptions/CSharpException.cpp
	WebFramework/src/Exceptions/AlreadyLoggedException.cpp
	WebFramework/src/Executors/ResourceExecutor.cpp
	WebFramework/src/LoadBalancer/LoadBalancerServer.cpp
	WebFramework/src/LoadBalancer/Heuristics/LoadBalancerHeuristic.cpp
	WebFramework/src/LoadBalancer/Heuristics/Connections.cpp
	WebFramework/src/LoadBalancer/Heuristics/CXXHeuristic.cpp
	WebFramework/src/LoadBalancer/Heuristics/CCHeuristic.cpp
	WebFramework/src/LoadBalancer/Heuristics/PythonHeuristic.cpp
	WebFramework/src/LoadBalancer/Heuristics/CSharpHeuristic.cpp
	WebFramework/src/Proxy/ProxyServer.cpp
	WebFramework/src/Utility/Sources.cpp
	WebFramework/src/Utility/RouteParameters.cpp
	WebFramework/src/Utility/ConnectionData.cpp
	WebFramework/src/Utility/DynamicLibraries.cpp
	WebFramework/src/Utility/CrashHandler.cpp
	WebFramework/src/Utility/Utils.cpp
	WebFramework/src/Utility/ReadOnlyBuffer.cpp
	WebFramework/src/Utility/WebFrameworkConcepts.cpp
	WebFramework/src/Managers/DatabasesManager.cpp
	WebFramework/src/Managers/ExecutorsManager.cpp
	WebFramework/src/Managers/RuntimesManager.cpp
	WebFramework/src/Managers/TaskBrokersManager.cpp
	WebFramework/src/Managers/TaskExecutorsManager.cpp
	WebFramework/src/Web/Servers/BaseWebServer.cpp
	WebFramework/src/Web/Servers/MultithreadedWebServer.cpp
	WebFramework/src/Web/Servers/ThreadPoolWebServer.cpp
	WebFramework/src/Web/Servers/ExecutorServer.cpp
	WebFramework/src/Framework/Config.cpp
	WebFramework/src/Framework/WebFramework.cpp
	WebFramework/src/C_API/WebFrameworkCApi.cpp
	WebFramework/src/C_API/WebFrameworkExecutorsCApi.cpp
	WebFramework/src/C_API/WebFrameworkUtilityCApi.cpp
	WebFramework/src/C_API/WebFrameworkLocalizationCApi.cpp
	WebFramework/src/C_API/WebFrameworkWebSocketExecutorsCApi.cpp
	WebFramework/src/Utility/LargeFileHandlers/BaseLargeBodyHandler.cpp
	WebFramework/src/Utility/LargeFileHandlers/ThreadPoolHandler.cpp
	WebFramework/src/Utility/LargeFileHandlers/MultithreadedHandler.cpp
	WebFramework/src/Utility/AdditionalServerSettings.cpp
	WebFramework/src/Utility/ExecutorsUtility.cpp
	WebFramework/src/Utility/TaskExecutorsSettings.cpp
	WebFramework/src/Web/HttpRequestImplementation.cpp
	WebFramework/src/Web/HttpResponseImplementation.cpp
	WebFramework/src/Databases/DatabaseImplementation.cpp
	WebFramework/src/Databases/TableImplementation.cpp
	WebFramework/src/Databases/SQLResultImplementation.cpp
	WebFramework/src/Databases/SQLValueImplementation.cpp
	WebFramework/src/Executors/CXXExecutor.cpp
	WebFramework/src/Executors/CCExecutor.cpp
	WebFramework/src/Executors/PythonExecutor.cpp
	WebFramework/src/Executors/CSharpExecutor.cpp
	WebFramework/src/TaskBroker/TaskBroker.cpp
	WebFramework/src/TaskBroker/InternalTaskBroker.cpp
	WebFramework/src/TaskBroker/RabbitMqTaskBroker.cpp
	WebFramework/src/TaskBroker/TaskConsumer.cpp
	WebFramework/src/TaskBroker/TaskExecutors/TaskExecutor.cpp
	WebFramework/src/TaskBroker/TaskExecutors/CXXTaskExecutor.cpp
	WebFramework/src/TaskBroker/TaskExecutors/CCTaskExecutor.cpp
	WebFramework/src/TaskBroker/TaskExecutors/PythonTaskExecutor.cpp
	WebFramework/src/TaskBroker/TaskExecutors/CSharpTaskExecutor.cpp
	WebFramework/src/Assets/AssetProvider.cpp
	WebFramework/src/Assets/DefaultAssetProvider.cpp
	WebFramework/src/Assets/SingleBinaryAssetProvider.cpp
	WebFramework/src/Assets/SingleBinaryAsset.cpp
)

if (${WITH_STACKTRACE})
	add_definitions(-D__WITH_STACKTRACE__)
endif()

if (UNIX)
	add_definitions(-D__LINUX__)
	target_compile_options(
		${PROJECT_NAME} PRIVATE
		-fvisibility=hidden
		-fno-omit-frame-pointer
	)
	
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)

	if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL aarch64)
		set(COMPILE_TARGET LinuxARM)
	else()
		set(COMPILE_TARGET Linux)
	endif()
	
	if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
		add_definitions(-D__ANDROID__)

		set(COMPILE_TARGET Android)

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti")
	else()
		add_link_options(-rdynamic)
	endif()
elseif (WIN32)
	add_definitions(-DNOMINMAX)
	add_definitions(-DWIN32_LEAN_AND_MEAN)

	set(COMPILE_TARGET Windows)

	list(APPEND 3RDPARTY_LIBS crypt32)
else()
	message(FATAL "Does not support ${CMAKE_SYSTEM_NAME}")
endif()

if (${WITH_UNITY_BUILD})
	set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
endif()

if (NOT "${COMPILE_TARGET}" STREQUAL "Android")
	if (${WITH_PYTHON_EXECUTORS})
		add_definitions(-D__WITH_PYTHON_EXECUTORS__)
	endif()

	if (${WITH_DOTNET_EXECUTORS})
		add_definitions(-D__WITH_DOTNET_EXECUTORS__)
	endif()
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
	if (UNIX)
		set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -g")
		set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -g")
	endif()
endif()

if (DEFINED ENV{MARCH} AND NOT "$ENV{MARCH}" STREQUAL "")
	target_compile_options(${PROJECT_NAME} PRIVATE -march=$ENV{MARCH})
endif()

if (CMAKE_GENERATOR MATCHES "Visual Studio")
	target_compile_options(${PROJECT_NAME} PUBLIC "/MP")
endif()

add_subdirectory(3rdparty)
add_subdirectory(WebFrameworkCore)
add_subdirectory(WebFrameworkExecutors)
add_subdirectory(BaseTCPServer)
add_subdirectory(ThreadPool)
add_subdirectory(INIParser)
add_subdirectory(Log)
add_subdirectory(SHA256)
add_subdirectory(UtilityLibrary)
add_subdirectory(Databases)

add_subdirectory(Networks) # has JSON as dependency
add_subdirectory(Localization) # has JSON as dependency
add_subdirectory(FileManager) # has ThreadPool as dependency

target_include_directories(
	${PROJECT_NAME} PUBLIC
	WebFramework/include/
	WebFrameworkCore/src/
	WebFrameworkExecutors/src/
	BaseTCPServer/src/
	INIParser/src/
	Log/src/
	SHA256/src/
	${ADDITIONAL_INCLUDES}
)

target_link_directories(
	${PROJECT_NAME} PUBLIC
	${SQLITE_LIBRARY_PATH}
	${HIREDIS_LIBRARY_PATH}
	${ADDITIONAL_LIB_DIRS}
)

target_link_libraries(
	${PROJECT_NAME}
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
	${3RDPARTY_LIBS}
	${ADDITIONAL_LIBS}
)

if (${AS_DEPENDENCY})
	install(TARGETS ${PROJECT_NAME} DESTINATION .)

	install(
		CODE [[
			set(SHARED_LIBRARIES "")

			list(APPEND SHARED_LIBRARIES FileManager Localization Log sqlite3)

			foreach(SHARED_LIBRARY IN LISTS SHARED_LIBRARIES)
				if (UNIX)
					file(COPY ${CMAKE_INSTALL_PREFIX}/lib/lib${SHARED_LIBRARY}.so DESTINATION DESTINATION ${CMAKE_INSTALL_PREFIX})
				else()
					file(COPY ${CMAKE_INSTALL_PREFIX}/dll/${SHARED_LIBRARY}.dll DESTINATION DESTINATION ${CMAKE_INSTALL_PREFIX})
				endif()
			endforeach()

			if (UNIX)
				file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/lib")
			else()
				file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/dll")
			endif()

			file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/assets")
		]]
	)
else()
	install(
		TARGETS ${PROJECT_NAME}
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION dll
	)

	install(FILES WebFrameworkConfig.cmake DESTINATION .)
	install(FILES FindWebFramework.cmake DESTINATION .)
	
	install(DIRECTORY WebFramework/certificates DESTINATION .)
	install(DIRECTORY schemas DESTINATION .)
endif()

install(
	CODE [[
		if (UNIX)
			file(GLOB_RECURSE FILES_TO_REMOVE "${CMAKE_INSTALL_PREFIX}/lib/*.a")

			list(REMOVE_ITEM FILES_TO_REMOVE "${CMAKE_INSTALL_PREFIX}/lib/libCC_API.a")
		else()
			file(GLOB_RECURSE FILES_TO_REMOVE "${CMAKE_INSTALL_PREFIX}/lib/*.lib")
			file(GLOB_RECURSE FILES_TO_REMOVE "${CMAKE_INSTALL_PREFIX}/lib/*.pdb")

			list(REMOVE_ITEM FILES_TO_REMOVE "${CMAKE_INSTALL_PREFIX}/lib/CC_API.lib")
		endif()

		file(GLOB_RECURSE FILES_TO_REMOVE "${CMAKE_INSTALL_PREFIX}/cmake/jwt-cpp*.cmake")

		if (FILES_TO_REMOVE)
			file(REMOVE ${FILES_TO_REMOVE})
		endif()

		file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/lib/vendor")
		file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/lib/cmake")
		file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
	]]
)
install(
	CODE [[
		file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}/include/")
	]]
)

if (${WITH_LANGUAGES_APIS})
	add_subdirectory(API)
endif()
