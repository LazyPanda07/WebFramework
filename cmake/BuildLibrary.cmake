cmake_minimum_required(VERSION 3.27.0)

function(build_cmake_library target_name depends_name git_repository git_tag install_prefix build_arguments)
	if (NOT EXISTS ${CMAKE_BINARY_DIR}/${target_name})
		execute_process(
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMAND git clone ${git_repository} ${target_name} -b ${git_tag} --recursive
		)
	endif()

	if (NOT EXISTS ${CMAKE_BINARY_DIR}/${target_name}/build)
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${target_name}/build
		)
	endif()

	add_custom_target(
		${target_name} ALL
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${target_name}/build
		COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${install_prefix} -G ${CMAKE_GENERATOR} ${${build_arguments}} ..
		COMMAND ${CMAKE_COMMAND} --build . -j --config ${CMAKE_BUILD_TYPE}
		COMMAND ${CMAKE_COMMAND} --install . --config ${CMAKE_BUILD_TYPE}
	)
	
	add_dependencies(
		${depends_name}
		${target_name}
	)
endfunction()
