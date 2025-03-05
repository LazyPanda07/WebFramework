cmake_minimum_required(VERSION 3.27.0)

project(Executors)

add_library(
	${PROJECT_NAME} SHARED
	src/BaseExecutor.cpp
	src/BaseHeavyOperationStatefulExecutor.cpp
	src/BaseHeavyOperationStatelessExecutor.cpp
	src/BaseStatefulExecutor.cpp
	src/BaseStatelessExecutor.cpp
	src/Utility/JSONSettingsParser.cpp
	src/Exceptions/BaseWebFrameworkException.cpp
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
)

install(
	TARGETS ${PROJECT_NAME} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/dll
)
