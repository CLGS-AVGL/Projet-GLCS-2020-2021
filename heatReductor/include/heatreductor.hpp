#pragma once

// library headers
#include <fieldreductor.hpp>

class HeatReductor:
	public FieldReductor
{
public:
	// see overridden function
	virtual void field_reduction(Distributed2DField& data) const override;
};
