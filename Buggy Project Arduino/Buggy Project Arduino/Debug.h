#pragma once
class Debug
{
public:
	Debug();
	~Debug();

	void warning(char warningMessage[]) { }
	void error(char errorMessage[]) { }
	void log(char logMessage[]) { }
};

