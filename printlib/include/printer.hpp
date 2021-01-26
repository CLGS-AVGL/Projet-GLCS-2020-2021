#pragma once

#include <iostream>
#include <ostream>

// library headers
#include <simulationobserver.hpp>

// Interface de Printer
class Printer : public SimulationObserver {
public:
  virtual void simulation_updated(const Distributed2DField &data,
                                  const Configuration &config) = 0;
};
