#pragma once

// local headers
#include "distributed2dfield.hpp"

class FieldReductor
{
public:
	/// The destructor
	virtual ~FieldReductor() = default;
	
	/** Apply a reduction to the field
	* @param data the local data block to reduction
	*/
	virtual void field_reduction( Distributed2DField& data ) const = 0;
	
};

