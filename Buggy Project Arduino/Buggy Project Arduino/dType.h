#pragma once
class dType
{
public:
	char type;
	char data[256];

	dType(char type, char data[256]);
	~dType();
};

