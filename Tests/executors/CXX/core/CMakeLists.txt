cmake_minimum_required(VERSION 3.27.0)

project(CoreExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/HelloExecutor.cpp
	src/AssetsExecutor.cpp
	src/ChunksExecutor.cpp
	src/DownloadExecutor.cpp
	src/LocalizationExecutor.cpp
	src/UploadMultipartExecutor.cpp
	src/UploadChunkedExecutor.cpp
	src/UploadOctetStreamExecutor.cpp
	src/TextGenerator.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include/
	${WEB_FRAMEWORK_INCLUDE}
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
