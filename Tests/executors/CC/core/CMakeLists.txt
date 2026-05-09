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
	src/DynamicResources.c
	src/TokenExecutor.c
	src/TokenGiverExecutor.c
	src/WebSocketEcho.c
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	WebFramework::CC_API
)

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
