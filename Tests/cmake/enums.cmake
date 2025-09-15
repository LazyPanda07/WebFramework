# Macro to declare an enum-like cache variable
macro(enum_option VAR DEFAULT ALLOWED_VALUES DESCRIPTION)
    # Declare the cache variable
    set(${VAR} "${DEFAULT}" CACHE STRING "${DESCRIPTION}")
    set_property(CACHE ${VAR} PROPERTY STRINGS ${ALLOWED_VALUES})

    # Validate the current value
    list(FIND ${ALLOWED_VALUES} "${${VAR}}" _index)
    if(_index EQUAL -1)
        message(FATAL_ERROR
            "Invalid value for ${VAR}: '${${VAR}}'. "
            "Allowed values: ${ALLOWED_VALUES}. "
            "Default is '${DEFAULT}'."
        )
    endif()
endmacro()

# Example usage
# set(ALLOWED_VALUES "Debug" "Release" "Profile")
# enum_option(MY_MODE Release ALLOWED_VALUES "Build mode selection")
