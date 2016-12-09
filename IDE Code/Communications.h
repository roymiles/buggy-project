/*
  Communications.h - Library for communicated to the base station / other Arduino
*/
#ifndef Communications_h
#define Communications_h

#include "Arduino.h"

#pragma once
class Communications
{
public:
	Communications();
	~Communications();

	// Send data to the base station
	bool send() {}

	// Check if the buggy is ready to start
	bool isStart();

	// Get the map data from the web url
	void getMap(char* oMap);

private:
	struct MemoryStruct {
		char *memory;
		size_t size;
	};

	// Convert the data to an appropriate format before sending
	char* toJson() {}
};

#endif

