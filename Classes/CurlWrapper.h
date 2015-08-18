#ifndef __CUBIC_CURLWRAPPER_H__
#define __CUBIC_CURLWRAPPER_H__

#include <string>
#include "cocos2d.h"
#include "curl/curl.h"

USING_NS_CC;

class CurlWrapper {
public:
    static std::string get(std::string uri);
};

#endif