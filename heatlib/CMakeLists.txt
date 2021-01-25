cmake_minimum_required(VERSION 3.18)

set(HDF5_PREFER_PARALLEL TRUE)

find_package(HDF5 REQUIRED COMPONENTS C HL)
find_package(MPI REQUIRED COMPONENTS CXX)

add_library(heatlib
		src/finitediffheatsolver.cpp
		src/fixedconditions.cpp
		src/filecondition.cpp
)
target_include_directories(heatlib PUBLIC include/)
target_link_libraries(heatlib PUBLIC hdf5::hdf5 hdf5::hdf5_hl)
target_link_libraries(heatlib PUBLIC MPI::MPI_CXX)
target_link_libraries(heatlib PUBLIC GLCS2020_project::baselib)
add_library(GLCS2020_project::heatlib ALIAS heatlib)
