#pragma once

#include <mpi.h>
#include <hdf5.h>
#include <hdf5_hl.h>


// library headers
#include <initialconditionner.hpp>

class FileConditions:
	public InitialConditionner
{
	double m_delta_t;

public:
	// see overridden function
  virtual void initial_condition( Distributed2DField& data,
				  const Configuration& config) const override;

};
