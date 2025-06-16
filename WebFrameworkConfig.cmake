if (NOT DEFINED PROJECT_LOCALIZATION_DIR)
    set(PROJECT_LOCALIZATION_DIR ${CMAKE_SOURCE_DIR})
endif()

if (NOT DEFINED WEB_FRAMEWORK_CC_API)
    set(WEB_FRAMEWORK_CC_API OFF)
endif()

if (${WEB_FRAMEWORK_CC_API})
    message("WebFramework CC API")
else()
    message("WebFramework CXX API")

    if (NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 20)
    endif()

    if (${CMAKE_CXX_STANDARD} LESS 20)
        set(CMAKE_CXX_STANDARD 20)
    endif()
endif()

if (UNIX)
    add_definitions(-D__LINUX__)
endif(UNIX)

if (NOT DEFINED CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME " ")
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_definitions(-D__ANDROID__)
endif()

if (CMAKE_SCRIPT_MODE_FILE)
    message("Script mode doesn't supported")

    return()
endif()

include_directories(
    BEFORE
    ${WEB_FRAMEWORK_SDK}/include/
    ${WEB_FRAMEWORK_SDK}/include/vendor/OpenSSL/
    ${WEB_FRAMEWORK_SDK}/include/vendor/sqlite3/
)

set(
    WEB_FRAMEWORK_LIBS
    WebFramework
    WebFrameworkCore
    WebFrameworkExecutors
    BaseTCPServer
    FileManager
    Log
    ThreadPool
    UtilityLibrary
    HTTP
    JSON
    SocketStreams
    Networks
    INIParser
    Localization
    SHA256
    ssl
    crypto
    sqlite3
)
set(IS_WEB_FRAMEWORK_SHARED OFF CACHE STRING "Is WebFramework dynamically linked")

link_directories(
    BEFORE
    ${WEB_FRAMEWORK_SDK}/lib/
    ${WEB_FRAMEWORK_SDK}/lib/vendor/OpenSSL/
    ${WEB_FRAMEWORK_SDK}/lib/vendor/sqlite3/
)

if (WIN32)
    set(
        WEB_FRAMEWORK_LIBS
        ${WEB_FRAMEWORK_LIBS}
        crypt32
        Rpcrt4
    )
elseif (UNIX AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(
        WEB_FRAMEWORK_LIBS
        ${WEB_FRAMEWORK_LIBS}
        uuid
    )
endif()

if (UNIX)
    install(
        DIRECTORY ${WEB_FRAMEWORK_SDK}/lib/ 
        DESTINATION ${CMAKE_INSTALL_PREFIX} 
        FILES_MATCHING 
        PATTERN "*.so"
        PATTERN "vendor" EXCLUDE
    )

    if(EXISTS ${WEB_FRAMEWORK_SDK}/lib/libWebFramework.so)
        set(IS_WEB_FRAMEWORK_SHARED ON)
    endif()
elseif(WIN32)
    install(DIRECTORY ${WEB_FRAMEWORK_SDK}/dll/ DESTINATION ${CMAKE_INSTALL_PREFIX})

    if(EXISTS ${WEB_FRAMEWORK_SDK}/dll/WebFramework.dll)
        set(IS_WEB_FRAMEWORK_SHARED ON)
    endif()
endif(UNIX)

if (${IS_WEB_FRAMEWORK_SHARED})
    if (${WEB_FRAMEWORK_CC_API})
        include_directories(
            BEFORE
            ${WEB_FRAMEWORK_SDK}/api/cc/include/
        )
    else()
        include_directories(
            BEFORE
            ${WEB_FRAMEWORK_SDK}/api/cxx/include/
        )
    endif()
endif()

if (NOT TARGET generate_localization)
    if ((NOT ${CMAKE_HOST_SYSTEM_PROCESSOR} EQUAL ${CMAKE_SYSTEM_PROCESSOR}) AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
        add_custom_target(
            generate_localization
            COMMAND qemu-aarch64 ${WEB_FRAMEWORK_SDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} generate
        )

        add_custom_target(
            build_debug_localization
            COMMAND qemu-aarch64 ${WEB_FRAMEWORK_SDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} debug_build ${DEBUG_LOCALIZATION_DIR}
        )

        add_custom_target(
            build_release_localization
            COMMAND qemu-aarch64 ${WEB_FRAMEWORK_SDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} release_build ${RELEASE_LOCALIZATION_DIR}
        )
    else()
        add_custom_target(
            generate_localization
            COMMAND ${WEB_FRAMEWORK_SDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} generate
        )

        add_custom_target(
            build_debug_localization
            COMMAND ${WEB_FRAMEWORK_SDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} debug_build ${DEBUG_LOCALIZATION_DIR}
        )

        add_custom_target(
            build_release_localization
            COMMAND ${WEB_FRAMEWORK_SDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} release_build ${RELEASE_LOCALIZATION_DIR}
        )
    endif()
endif()
