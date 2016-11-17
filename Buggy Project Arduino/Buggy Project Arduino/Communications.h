#include <Debug.h>
#include <dType.h>

#pragma once
class Communications
{
public:
	Communications(Debug *d);
	~Communications();

	// Send data to the base station
	bool send(dType data) {}

	// Check if the buggy is ready to start
	bool isStart();

	// Get the map data from the web url
	char* getMap();

private:
	Debug *d;

	// Convert the data to an appropriate format before sending
	char* toJson(dType data) {}
};

