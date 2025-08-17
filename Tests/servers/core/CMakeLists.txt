cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
	file(CHMOD ${WEB_FRAMEWORK_LIBRARY_DIR}/assets/LocalizationUtils PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
endif(UNIX)

project(Server)

add_executable(
	${PROJECT_NAME}
	main.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_INCLUDE}
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
