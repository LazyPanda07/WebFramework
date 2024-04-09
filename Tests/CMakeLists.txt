cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)
set(WEB_FRAMEWORK_LIBRARY_PATH ../WebFrameworkLibrary)
set(
	LIBS 
	WebFramework
	BaseTCPServer
	JSON
	FileManager
	ThreadPool
	Log
	HTTP
	Networks
	SocketStreams
	UtilityLibrary
	sqlite3
	ssl
	crypto
)

if (UNIX)
	add_definitions(-D__LINUX__)

	set(CMAKE_CXX_FLAGS -rdynamic)
else ()
	add_definitions(-DNOMINMAX)

	set(LIBS ${LIBS} crypt32)
endif (UNIX)

project(Tests)

add_executable(
	${PROJECT_NAME}
	main.cpp
	utilities/utilities.cpp
	executors/HelloExecutor.cpp
	cases/GetRequest.cpp
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	utilities/
	../WebFramework/src/
	../BaseTCPServer/src/
	../Networks/src/
	../Networks/HTTP/src/
	../Networks/HTTP/JSON/src/
	../FileManager/src/
	../ThreadPool/src/
	../Log/src/
	../SHA256/src/
	../INIParser/src/
	../Localization/src/
	../Networks/SocketStreams/src/
	../UtilityLibrary/src/
	../Networks/vendor/OpenSSL/include/
	../WebFramework/vendor/sqlite3/include/
	gtest/include/
)

target_link_directories(
	${PROJECT_NAME} PUBLIC
	${WEB_FRAMEWORK_LIBRARY_PATH}/lib/
	gtest/lib/
)

target_link_libraries(
	${PROJECT_NAME} PUBLIC
	${LIBS}
	gtest
	gtest_main
)

if (UNIX)
	install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_PATH}/lib/ DESTINATION ${CMAKE_BINARY_DIR}/bin FILES_MATCHING PATTERN "*.so")
else ()
	install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_PATH}/dll/ DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif (UNIX)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(DIRECTORY configs/ DESTINATION ${CMAKE_BINARY_DIR}/bin)

install(DIRECTORY ${WEB_FRAMEWORK_LIBRARY_PATH}/certificates DESTINATION ${CMAKE_BINARY_DIR}/bin)
