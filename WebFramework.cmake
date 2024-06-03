if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

if (${CMAKE_CXX_STANDARD} LESS 20)
    set(CMAKE_CXX_STANDARD 20)
endif()

if (UNIX)
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

link_directories(
    BEFORE
    ${WebFrameworkSDK}/lib/
    ${WebFrameworkSDK}/lib/vendor/OpenSSL/
    ${WebFrameworkSDK}/lib/vendor/sqlite3/
)

if (WIN32)
    set(
        WEB_FRAMEWORK_LIBS
        ${WEB_FRAMEWORK_LIBS}
        crypt32
    )
endif(WIN32)

if (UNIX)
    install(DIRECTORY ${WebFrameworkSDK}/lib/ DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.so")
elseif(WIN32)
    install(DIRECTORY ${WebFrameworkSDK}/dll/ DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(UNIX)
