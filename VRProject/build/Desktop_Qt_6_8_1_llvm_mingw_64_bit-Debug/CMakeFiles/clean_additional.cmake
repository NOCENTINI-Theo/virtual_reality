# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\VRProject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\VRProject_autogen.dir\\ParseCache.txt"
  "VRProject_autogen"
  )
endif()
