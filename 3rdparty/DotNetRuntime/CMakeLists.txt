cmake_minimum_required(VERSION 3.27.0)

set(DOTNET_RUNTIME_VERSION 8)

project(external_dot_net)

if (UNIX)
	set(BASE_DOTNET_HOST_SDK_PATH "/usr/lib/dotnet/packs/Microsoft.NETCore.App.Host.*-x64" CACHE STRING "")
else()
	set(BASE_DOTNET_HOST_SDK_PATH "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64" CACHE STRING "")
endif()

file(GLOB DOTNET_SDKS "${BASE_DOTNET_HOST_SDK_PATH}/*")
list(SORT DOTNET_SDKS COMPARE NATURAL ORDER DESCENDING)
list(FILTER DOTNET_SDKS INCLUDE REGEX "${DOTNET_RUNTIME_VERSION}.*.*")

list(LENGTH DOTNET_SDKS DOTNET_SDKS_SIZE)

if (${DOTNET_SDKS_SIZE} EQUAL 0)
	message(FATAL_ERROR "Can't find .NET ${DOTNET_RUNTIME_VERSION}")
endif()

list(GET DOTNET_SDKS 0 DOTNET_SDK)

string(APPEND DOTNET_SDK "/runtimes")

file(GLOB DOTNET_SDK "${DOTNET_SDK}/*")

string(APPEND DOTNET_SDK "/native")

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	list(APPEND ADDITIONAL_INCLUDES ${DOTNET_SDK})
else()
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

	file(COPY ${DOTNET_SDK}/hostfxr.h DESTINATION ${CMAKE_BINARY_DIR}/include)
	file(COPY ${DOTNET_SDK}/coreclr_delegates.h DESTINATION ${CMAKE_BINARY_DIR}/include)
endif()

set(ADDITIONAL_INCLUDES ${ADDITIONAL_INCLUDES} CACHE INTERNAL "")
