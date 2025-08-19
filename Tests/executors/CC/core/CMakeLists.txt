cmake_minimum_required(VERSION 3.27.0)

project(CoreExecutors)

add_library(
	${PROJECT_NAME} SHARED
	src/HelloExecutor.c
	src/AssetsExecutor.c
	src/ChunksExecutor.c
	src/DownloadExecutor.c
	src/LocalizationExecutor.c
	src/UploadMultipartExecutor.c
	src/UploadChunkedExecutor.c
	src/UploadOctetStreamExecutor.c
	src/TextGenerator.c
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include/
	${WEB_FRAMEWORK_INCLUDE}
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
