cmake_minimum_required(VERSION 3.18)

project(GLCS2020_project CXX)

find_package(MPI REQUIRED COMPONENTS CXX)
find_package(Boost 1.56 REQUIRED COMPONENTS program_options)

add_subdirectory("vendor/mdspan/" "mdspan" EXCLUDE_FROM_ALL)

add_subdirectory(baselib)
add_subdirectory(heatlib)
add_subdirectory(simpleui)
add_subdirectory(configReader)

add_executable(simpleheat
		simpleheat.cpp
)
target_link_libraries(simpleheat PRIVATE
		GLCS2020_project::baselib
		GLCS2020_project::heatlib
		GLCS2020_project::simpleui
    GLCS2020_project::configReader
		MPI::MPI_CXX
    Boost::program_options)
