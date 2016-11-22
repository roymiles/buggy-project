#include <Debug.h>
#include <dType.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <curl/curl.h>

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
	struct MemoryStruct {
		char *memory;
		size_t size;
	};

	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct *mem = (struct MemoryStruct *)userp;

		mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory == NULL) {
			/* out of memory! */
			printf("not enough memory (realloc returned NULL)\n");
			return 0;
		}

		memcpy(&(mem->memory[mem->size]), contents, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;

		return realsize;
	}

	// Convert the data to an appropriate format before sending
	char* toJson(dType data) {}
};

