cmake_minimum_required(VERSION 3.18)

add_library(heatReductor
		src/heatreductor.cpp
)

target_include_directories(heatReductor PUBLIC include/)
target_link_libraries(heatReductor PUBLIC GLCS2020_project::baselib)
add_library(GLCS2020_project::heatReductor ALIAS heatReductor)
