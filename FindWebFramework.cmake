if (NOT DEFINED WEB_FRAMEWORK_SDK)
    set(WEB_FRAMEWORK_SDK $ENV{WEB_FRAMEWORK_SDK})
endif()

if (NOT DEFINED WEB_FRAMEWORK_SDK)
    if (UNIX)
        if (EXISTS /usr/lib/web-framework/WebFrameworkConfig.cmake)
            set(WEB_FRAMEWORK_SDK /usr/lib/web-framework)
        endif()
    endif()
endif()

if (NOT DEFINED WEB_FRAMEWORK_SDK)
    message(FATAL_ERROR "WEB_FRAMEWORK_SDK variable doesn't set")
endif()

include(${WEB_FRAMEWORK_SDK}/WebFrameworkConfig.cmake)
