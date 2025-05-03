# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FixEngineLib_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FixEngineLib_autogen.dir\\ParseCache.txt"
  "Executable\\CMakeFiles\\Executable_autogen.dir\\AutogenUsed.txt"
  "Executable\\CMakeFiles\\Executable_autogen.dir\\ParseCache.txt"
  "Executable\\Executable_autogen"
  "FixEngineLib_autogen"
  "Test\\CMakeFiles\\TestFixEngine_autogen.dir\\AutogenUsed.txt"
  "Test\\CMakeFiles\\TestFixEngine_autogen.dir\\ParseCache.txt"
  "Test\\TestFixEngine_autogen"
  )
endif()
