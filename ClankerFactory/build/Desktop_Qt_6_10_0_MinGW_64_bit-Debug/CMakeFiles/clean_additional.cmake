# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ClankerFactory_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ClankerFactory_autogen.dir\\ParseCache.txt"
  "ClankerFactory_autogen"
  )
endif()
