#include <dType.h>

#pragma once
class Communications
{
public:
	Communications();
	~Communications();

	// Send data to the base station
	bool send(dType data) {}

	// Convert the data to an appropriate format before sending
	char* toJson(dType data) {}

	// Check if the buggy is ready to start
	bool isStart();

private:
};

