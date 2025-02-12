# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\CandidateFIlter_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CandidateFIlter_autogen.dir\\ParseCache.txt"
  "CandidateFIlter_autogen"
  )
endif()
