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
	src/DynamicResources.cpp
	src/TextGenerator.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    WebFramework::CXX_API
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
