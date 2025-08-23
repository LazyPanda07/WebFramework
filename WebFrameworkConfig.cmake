if (NOT DEFINED PROJECT_LOCALIZATION_DIR)
    set(PROJECT_LOCALIZATION_DIR ${CMAKE_SOURCE_DIR})
endif()

if (NOT DEFINED DEBUG_LOCALIZATION_DIR)
    set(DEBUG_LOCALIZATION_DIR ${CMAKE_INSTALL_PREFIX})
endif()

if (NOT DEFINED RELEASE_LOCALIZATION_DIR)
    set(RELEASE_LOCALIZATION_DIR ${CMAKE_INSTALL_PREFIX})
endif()

if (NOT DEFINED WEB_FRAMEWORK_CC_API)
    set(WEB_FRAMEWORK_CC_API OFF)
endif()

if (${WEB_FRAMEWORK_CC_API})
    if (NOT DEFINED CMAKE_C_STANDARD)
        set(CMAKE_C_STANDARD 11)
    endif()

    if (${CMAKE_C_STANDARD} LESS 11)
        set(CMAKE_C_STANDARD 11)
    endif()

    link_libraries(CC_API)

    if (NOT PROJECT_NAME)
        message(STATUS "WebFramework CC API")
    else()
        message(STATUS "${PROJECT_NAME} uses WebFramework CC API")
    endif()
else()
    if (NOT PROJECT_NAME)
        message(STATUS "WebFramework CXX API used")
    else()
        message(STATUS "${PROJECT_NAME} uses WebFramework CXX API")
    endif()

    set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(
    WEB_FRAMEWORK_INCLUDE
    ${WEB_FRAMEWORK_SDK}/include/
    ${WEB_FRAMEWORK_SDK}/include/vendor/OpenSSL/
    ${WEB_FRAMEWORK_SDK}/include/vendor/sqlite3/
)

set(
    WEB_FRAMEWORK_LIB
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

link_directories(
    BEFORE
    ${WEB_FRAMEWORK_SDK}/lib/
    ${WEB_FRAMEWORK_SDK}/lib/vendor/OpenSSL/
    ${WEB_FRAMEWORK_SDK}/lib/vendor/sqlite3/
)

if (WIN32)
    set(
        WEB_FRAMEWORK_LIB
        ${WEB_FRAMEWORK_LIB}
        crypt32
        Rpcrt4
    )
elseif (UNIX AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(
        WEB_FRAMEWORK_LIB
        ${WEB_FRAMEWORK_LIB}
        uuid
    )
endif()

if (UNIX)
    install(
        DIRECTORY ${WEB_FRAMEWORK_SDK}/lib/ 
        DESTINATION .
        FILES_MATCHING 
        PATTERN "*.so"
        PATTERN "vendor" EXCLUDE
    )
elseif(WIN32)
    install(DIRECTORY ${WEB_FRAMEWORK_SDK}/dll/ DESTINATION .)
endif(UNIX)

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
