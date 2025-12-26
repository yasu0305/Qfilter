# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/filteredcompleter_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/filteredcompleter_autogen.dir/ParseCache.txt"
  "CMakeFiles/filterutils_test_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/filterutils_test_autogen.dir/ParseCache.txt"
  "CMakeFiles/hello_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/hello_autogen.dir/ParseCache.txt"
  "filteredcompleter_autogen"
  "filterutils_test_autogen"
  "hello_autogen"
  )
endif()
