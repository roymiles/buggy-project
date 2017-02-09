/**
    Communication.h
    Purpose: This is a generic interface used by all submodules
			 that will be communicating with other components

    @author Roy Miles
    @version 1.0 11/12/2016
*/

#ifndef Communication_h
#define Communication_h

#include "Arduino.h"
#include "dType.h"

#pragma once
class Communication
{
public:
	Communication();
	~Communication();

	// Send data
	virtual bool send(dType d);

	// Get data
	virtual bool get(dType d);
};

#endif

