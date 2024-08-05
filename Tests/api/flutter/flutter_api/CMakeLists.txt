cmake_minimum_required(VERSION 3.27.0)

project(flutter_api)

add_custom_target(
	${PROJECT_NAME} ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND flutter build ${BUILD_PLATFORM} --release
)
