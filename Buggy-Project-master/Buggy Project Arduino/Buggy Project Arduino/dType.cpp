#include "dType.h"

dType::dType(char type, char data[256])
{
	dType::type = type;
	dType::data[256] = *data; // Data is at max 256 characters long
}


dType::~dType()
{
}
