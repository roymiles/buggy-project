#pragma once
class dType
{
public:
	char type;
	char data[];

	dType(char type, char data[]);
	~dType();
};

