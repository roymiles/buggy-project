#include "Communications.h"

Communications::Communications(Debug *d)
{
	Communications::d = d;
}


Communications::~Communications()
{
}

bool Communications::isStart()
{
	return false;
}

char* Communications::getMap()
{
	char map[10] = "abcdefghi";
	return map;
}