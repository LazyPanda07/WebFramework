function(generate_json input_path output_path replaces)
	set(REPLACES "${${replaces}}")
  set(i 0)
  list(LENGTH REPLACES SIZE)
  math(EXPR REM "${SIZE} % 2")

  if(${REM})
      message(FATAL_ERROR "Expected an even number of items (pairs), got ${SIZE}.")
  endif()

  file(READ ${input_path} FILE_CONTENT)

  while(${i} LESS ${SIZE})
      math(EXPR j "${i} + 1")

      list(GET REPLACES ${i} KEY)
      list(GET REPLACES ${j} VALUE)
    
      string(REPLACE ${KEY} ${VALUE} FILE_CONTENT ${FILE_CONTENT})

      math(EXPR i "${i} + 2")
  endwhile()

	file(WRITE ${output_path} ${FILE_CONTENT})
endfunction()
