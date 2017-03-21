#include "Communications.h"

Communications::Communications(Debug *d)
{
}


Communications::~Communications()
{
}

bool Communications::isStart()
{
	/*
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;

	chunk.memory = (char *)malloc(1);  // will be grown as needed by the realloc above
	chunk.size = 0;    // no data at this point

	curl_global_init(CURL_GLOBAL_ALL);

	// init the curl session
	curl_handle = curl_easy_init();

	// specify URL to get
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://www.example.com/");

	// send all data to this function
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	// we pass our 'chunk' struct to the callback function
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	// some servers don't like requests that are made without a user-agent field, so we provide one
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	// get it!
	res = curl_easy_perform(curl_handle);

	// check for errors
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
	}
	else {
		/*
		* Now, our chunk.memory points to a memory block that is chunk.size
		* bytes big and contains the remote file.
		*
		* Do something nice with it!
		*//*
		std::cout << chunk.memory;
		printf("%lu bytes retrieved\n", (long)chunk.size);
	} 

	// cleanup curl stuff
	curl_easy_cleanup(curl_handle);

	free(chunk.memory);

	// we're done with libcurl, so clean it up
	curl_global_cleanup();
	*/
	return 0;
}

char* Communications::getMap()
{
	char map[10] = "abcdefghi";
	return map;
}