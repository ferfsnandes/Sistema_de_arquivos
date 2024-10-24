# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\sistema_de_arquivos_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\sistema_de_arquivos_autogen.dir\\ParseCache.txt"
  "sistema_de_arquivos_autogen"
  )
endif()
