cmake_minimum_required(VERSION 3.18)


set(HDF5_PREFER_PARALLEL TRUE)
find_package(HDF5 REQUIRED COMPONENTS C HL)

find_package(MPI REQUIRED COMPONENTS CXX)

add_library(printlib
		src/HDF5printer.cpp
		src/meanprinter.cpp
)
target_include_directories(printlib PUBLIC include/)
target_link_libraries(printlib PUBLIC GLCS2020_project::baselib)
target_link_libraries(printlib PUBLIC hdf5::hdf5 hdf5::hdf5_hl)
target_link_libraries(printlib PUBLIC MPI::MPI_CXX)
add_library(GLCS2020_project::printlib ALIAS printlib)
