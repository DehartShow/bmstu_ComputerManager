# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/bmstu_ComputerManager_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/bmstu_ComputerManager_autogen.dir/ParseCache.txt"
  "bmstu_ComputerManager_autogen"
  )
endif()
