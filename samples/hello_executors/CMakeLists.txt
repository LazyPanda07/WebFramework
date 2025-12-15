cmake_minimum_required(VERSION 3.27.0)

function(list_to_string_array list_variable)
	set(output_variable ${list_variable})
	set(list_variable ${${list_variable}})

	list(TRANSFORM list_variable PREPEND "\"")
	list(TRANSFORM list_variable APPEND "\"")
	
	list(TRANSFORM list_variable PREPEND "      ")
	
	string(JOIN ",\n" list_variable ${list_variable})

	set(${output_variable} ${list_variable} PARENT_SCOPE)
endfunction()

project(hello_executors)

set(SETTINGS "")
set(LOAD_SOURCES "")
set(RUNTIMES "")

if (${CXX_SAMPLE})
	add_subdirectory(cxx)

	list(APPEND SETTINGS cxx_web.json)
	list(APPEND LOAD_SOURCES cxx_hello_executor)
endif()

if (${CC_SAMPLE})
	add_subdirectory(cc)

	list(APPEND SETTINGS cc_web.json)
	list(APPEND LOAD_SOURCES cc_hello_executor)
endif()

if (${PYTHON_SAMPLE})
	add_subdirectory(python)

	list(APPEND SETTINGS python_web.json)
	list(APPEND LOAD_SOURCES python_hello_executor)
	list(APPEND RUNTIMES python)
endif()

if (${CSHARP_SAMPLE})
	add_subdirectory(csharp)

	list(APPEND SETTINGS csharp_web.json)
	list(APPEND LOAD_SOURCES csharp_hello_executor)
	list(APPEND RUNTIMES .net)
endif()

file(READ configs/config.json OUTPUT_DATA)

list(LENGTH RUNTIMES RUNTIMES_LENGTH)

list_to_string_array(SETTINGS)
list_to_string_array(LOAD_SOURCES)
list_to_string_array(RUNTIMES)

string(REPLACE "{SETTINGS}" ${SETTINGS} OUTPUT_DATA ${OUTPUT_DATA})
string(REPLACE "{LOAD_SOURCES}" ${LOAD_SOURCES} OUTPUT_DATA ${OUTPUT_DATA})
string(REPLACE "{RUNTIMES}" ${RUNTIMES} OUTPUT_DATA ${OUTPUT_DATA})

file(WRITE ${CMAKE_INSTALL_PREFIX}/configs/config.json ${OUTPUT_DATA})
