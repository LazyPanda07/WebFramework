cmake_minimum_required(VERSION 3.27.0)

if (UNIX)

else()
	list(APPEND ADDITIONAL_INCLUDES "C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Host.win-x64/8.0.22/runtimes/win-x64/native")
endif()

set(ADDITIONAL_INCLUDES ${ADDITIONAL_INCLUDES} CACHE INTERNAL "")
