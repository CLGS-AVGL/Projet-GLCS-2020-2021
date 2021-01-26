#pragma once

// headers
#include <hdf5.h>
#include <hdf5_hl.h>
#include <mpi.h>
#include <printer.hpp>

// classe printer HDF5
class HDF5printer : public Printer {
public:
  void simulation_updated(const Distributed2DField &data,
                          const Configuration &config) override;
};
