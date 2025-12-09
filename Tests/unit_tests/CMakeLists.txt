cmake_minimum_required(VERSION 3.27.0)

set(UTILITY_LIBRARY_VERSION 0.8.0)
set(NETWORKS_VERSION 1.22.6)
set(LOCALIZATION_VERSION 1.4.6)

project(test_suites)

if(UNIX)
	add_definitions(-D__LINUX__)
endif(UNIX)

FetchContent_Declare(
	ExternalLocalization
	GIT_REPOSITORY https://github.com/LazyPanda07/Localization.git
	GIT_TAG v${LOCALIZATION_VERSION}
)
FetchContent_Declare(
	ExternalUtilityLibrary
	GIT_REPOSITORY https://github.com/LazyPanda07/UtilityLibrary.git
	GIT_TAG v${UTILITY_LIBRARY_VERSION}
)
FetchContent_Declare(
	ExternalNetworks
	GIT_REPOSITORY https://github.com/LazyPanda07/Networks.git
	GIT_TAG v${NETWORKS_VERSION}
)

FetchContent_MakeAvailable(ExternalUtilityLibrary)
FetchContent_MakeAvailable(ExternalNetworks)
FetchContent_MakeAvailable(ExternalLocalization)

add_subdirectory(core)
add_subdirectory(load_balancer)
add_subdirectory(proxy)
