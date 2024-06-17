if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

if(NOT DEFINED PROJECT_LOCALIZATION_DIR)
    set(PROJECT_LOCALIZATION_DIR ${CMAKE_SOURCE_DIR})
endif()

if(${CMAKE_CXX_STANDARD} LESS 20)
    set(CMAKE_CXX_STANDARD 20)
endif()

if(UNIX)
    add_definitions(-D__LINUX__)
endif(UNIX)

include_directories(
    BEFORE
    ${WebFrameworkSDK}/include/
    ${WebFrameworkSDK}/include/vendor/OpenSSL/
    ${WebFrameworkSDK}/include/vendor/sqlite3/
)

set(
    WEB_FRAMEWORK_LIBS
    WebFramework
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
    ${WebFrameworkSDK}/lib/
    ${WebFrameworkSDK}/lib/vendor/OpenSSL/
    ${WebFrameworkSDK}/lib/vendor/sqlite3/
)

if(UNIX)
    set(
        WEB_FRAMEWORK_LIBS
        ${WEB_FRAMEWORK_LIBS}
        uuid
    )
elseif(WIN32)
    set(
        WEB_FRAMEWORK_LIBS
        ${WEB_FRAMEWORK_LIBS}
        crypt32
        Rpcrt4
    )
endif()

if(UNIX)
    install(DIRECTORY ${WebFrameworkSDK}/lib/ DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.so")

    if(EXISTS ${WebFrameworkSDK}/lib/libWebFramework.so)
        set(IS_WEB_FRAMEWORK_SHARED ON)
    endif()
elseif(WIN32)
    install(DIRECTORY ${WebFrameworkSDK}/dll/ DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN "vendor" EXCLUDE)
    install(FILES ${WebFrameworkSDK}/dll/vendor/sqlite3/sqlite3.dll DESTINATION ${CMAKE_INSTALL_PREFIX})

    if(EXISTS ${WebFrameworkSDK}/dll/WebFramework.dll)
        set(IS_WEB_FRAMEWORK_SHARED ON)
    endif()
endif(UNIX)

if(NOT TARGET generate_localization)
    add_custom_target(
        generate_localization
        COMMAND ${WebFrameworkSDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} generate
    )

    add_custom_target(
        build_debug_localization
        COMMAND ${WebFrameworkSDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} debug_build ${DEBUG_LOCALIZATION_DIR}
    )

    add_custom_target(
        build_release_localization
        COMMAND ${WebFrameworkSDK}/assets/LocalizationUtils ${PROJECT_LOCALIZATION_DIR} release_build ${RELEASE_LOCALIZATION_DIR}
    )
endif()
