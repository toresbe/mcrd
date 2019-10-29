#include "HTTPGetter.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <vector>
#include <curl/curl.h>

void HTTPGetter::init_curl() {
    std::lock_guard<std::mutex> lk(curl_threadsafe);
    if (!curl_instance_count++) {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    /* init the curl session */
    curl_handle = curl_easy_init();

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &new_data_callback);

    /* we pass our instance to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)this);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "mcrd/1.0");

}

void HTTPGetter::deinit_curl() {
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    std::lock_guard<std::mutex> lk(curl_threadsafe);

    if (!--curl_instance_count) {
        /* we're done with libcurl, so clean it up */
        curl_global_cleanup();
    }
}

size_t HTTPGetter::new_data_callback(void* contents, size_t size, size_t nmemb, void* this_p)
{
    auto instance = (HTTPGetter*)this_p;

    size_t realsize = size * nmemb;

    std::vector<uint8_t> new_data((uint8_t*)contents, (uint8_t*)contents + realsize);
    instance->buffer.insert(instance->buffer.end(), new_data.begin(), new_data.end());

    return realsize;
}


HTTPGetter::HTTPGetter(std::string uri) {
    this->uri = uri;
    init_curl();
    curl_easy_setopt(curl_handle, CURLOPT_URL, uri.c_str());
}

HTTPGetter::~HTTPGetter() {
    deinit_curl();
}

std::string HTTPGetter::get() {
    buffer.clear();

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
    }

    return std::string(buffer.begin(), buffer.end());
};
