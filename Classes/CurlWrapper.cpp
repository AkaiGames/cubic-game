#include "CurlWrapper.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

std::string CurlWrapper::get(std::string uri)
{
    std::stringstream result;
    CURL *curl;
    CURLcode res;
    char buffer[10];

    struct MemoryStruct chunk;
    
    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl = curl_easy_init();
    
    if (curl) 
    { 
        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str()); 
        //code from http://curl.haxx.se/libcurl/c/getinmemory.html 
        /* we pass our 'chunk' struct to the callback function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); 
        //If we don't provide a write function for curl, it will recieve error code 23 on windows. 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); 

        res = curl_easy_perform(curl); 
        /* always cleanup */ 
        curl_easy_cleanup(curl); 
        
        if (res == 0) 
        { 
            result << chunk.memory; 
        } 
        else 
        { 
            result << "error: " << curl_easy_strerror(res); 
        } 
    } 
    else 
    { 
        result << "no curl"; 
    } 
    
    return result.str(); 
}

std::string CurlWrapper::post(std::string uri, std::string params)
{
  std::stringstream result;
    CURL *curl;
    CURLcode res;
    char buffer[10];

    struct MemoryStruct chunk;
    
    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl = curl_easy_init();
    
    if (curl) 
    { 
        curl_easy_setopt(curl, CURLOPT_URL, uri.c_str()); 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
        //code from http://curl.haxx.se/libcurl/c/getinmemory.html 
        /* we pass our 'chunk' struct to the callback function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); 
        //If we don't provide a write function for curl, it will recieve error code 23 on windows. 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); 

        res = curl_easy_perform(curl); 
        /* always cleanup */ 
        curl_easy_cleanup(curl); 
        
        if (res == 0) 
        { 
            result << chunk.memory; 
        } 
        else 
        { 
            result << "error: " << curl_easy_strerror(res); 
        } 
    } 
    else 
    { 
        result << "no curl"; 
    } 
    
    return result.str(); 
}

