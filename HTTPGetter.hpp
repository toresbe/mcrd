#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <vector>
#include <curl/curl.h>

class HTTPGetter {
private:
	std::string uri;
	std::vector<uint8_t> buffer;
	std::mutex curl_threadsafe;
	CURL* curl_handle;
	CURLcode res;
	static inline int curl_instance_count = 0;

	void init_curl();
	void deinit_curl();
	static size_t new_data_callback(void* contents, size_t size, size_t nmemb, void* this_p);
public:
	HTTPGetter(std::string uri);
	~HTTPGetter();
	std::string get();
};