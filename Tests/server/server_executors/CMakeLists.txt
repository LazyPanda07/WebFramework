cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(ServerExecutors)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

add_library(
	${PROJECT_NAME} SHARED
	TextGenerator.cpp
	executors/HelloExecutor.cpp
	executors/AssetsExecutor.cpp
	executors/ChunksExecutor.cpp
	executors/DownloadExecutor.cpp
	executors/LocalizationExecutor.cpp
	executors/UploadMultipartExecutor.cpp
	executors/UploadChunkedExecutor.cpp
	executors/UploadOctetStreamExecutor.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	${WEB_FRAMEWORK_INCLUDES}
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	Log
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
