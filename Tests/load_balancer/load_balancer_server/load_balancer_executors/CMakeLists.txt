cmake_minimum_required(VERSION 3.27.0)

include(${WEB_FRAMEWORK_LIBRARY_DIR}/WebFrameworkConfig.cmake)

project(LoadBalancerExecutors)

add_library(
	${PROJECT_NAME} SHARED
	IdExecutor.cpp
	CustomHeuristic.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_SDK}/api/cxx/include
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
