# Test if source files in a directory compile

include( CheckCSourceCompiles )

function(test_compile_all_c)
   file(GLOB all_cfile "*.c")
   foreach( cfile ${all_cfile})
      file(READ ${cfile} cfile_source)
      CHECK_C_SOURCE_COMPILES("${cfile_source}" ${cfile}_WORKS)
   endforeach( cfile ${all_cfile})
endfunction(test_compile_all_c)