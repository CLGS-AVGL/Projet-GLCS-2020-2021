#include <mpi.h>

#include "HDF5printer.hpp"
#include "configReader.hpp"
#include "fileconditions.hpp"
#include "finitediffheatsolver.hpp"
#include "fixedconditions.hpp"
#include "heatreductor.hpp"
#include "meanprinter.hpp"
#include "screenprinter.hpp"
#include "simulation.hpp"

int main(int argc, char *argv[]) {
  // initialize the MPI library
  MPI_Init(&argc, &argv);

  // Construct the command-line arguments parser
  ConfigReader config(argc, argv);
  // construct the heat equation solver
  FinitediffHeatSolver heat_solver(config);

  // create a fixed initial conditionner
  FixedConditions fixed_init;

  // create a file initial conditionner
  FileConditions file_init;

  // create a reductor
  HeatReductor reduc;

  // construct the simulation runner with the good initial conditionner
  Simulation simulation =
      (config.input_filename().empty())
          ? Simulation(MPI_COMM_WORLD, config, heat_solver, fixed_init, reduc)
          : Simulation(MPI_COMM_WORLD, config, heat_solver, file_init, reduc);

  // add the standard output observer if no output file is given
  if (config.output_filename().empty()) {
    ScreenPrinter screen_printer;
    simulation.observe(screen_printer);
  }
  // add the mean and hdf5 printer if an output file is given
  else {
    HDF5printer hdf5_printer;
    MeanPrinter mean_printer;
    simulation.observe(hdf5_printer);
    simulation.observe(mean_printer);
  }

  // run the simulation
  simulation.run();

  // finalize MPI
  MPI_Finalize();

  return 0;
}
