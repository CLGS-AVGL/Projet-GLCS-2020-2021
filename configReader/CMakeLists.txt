cmake_minimum_required(VERSION 3.18)

add_library(configReader
  src/configReader.cpp
)
target_include_directories(configReader PUBLIC include/)
target_link_libraries(configReader PUBLIC GLCS2020_project::baselib)
add_library(GLCS2020_project::configReader ALIAS configReader)
