#pragma once

// local headers
#include "distributed2dfield.hpp"
#include "configuration.hpp"

class SimulationObserver
{
public:
	/// The destructor
	~SimulationObserver() = default;
	
	/** Notification when the state of the simulation changes
	 * @param data the new state of the simulation
	 */
  virtual void simulation_updated( const Distributed2DField& data ,
				   const Configuration& config) = 0;
	
};
