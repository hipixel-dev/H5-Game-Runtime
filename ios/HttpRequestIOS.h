//
//  HttpRequestIOS.h
//  wuwanlib
//
//  Created by 陈JH on 15/8/17.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#ifndef __wuwanlib__HttpRequestIOS__
#define __wuwanlib__HttpRequestIOS__

#include "../HttpRequestInterface.h"

class HttpRequestIOS: public HttpRequestInterface {
public:
    HttpRequestIOS();
    virtual ~HttpRequestIOS();

    /* send HTTP GET Request and get data returned by server, data MUST be released by caller.
     * return : 200 if success, else return 404.
     */
    //virtual int SendGetRequest(const char*szURL, RESPONSETYPE respType, const char*szReferer, char*&pDataRet, int &nRetLen);
    //virtual int SendPostRequest(const char*szURL, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen);
    virtual int SendPostOrGetRequest(const char*szURL, const char*szType, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen);
    
    
};

#endif /* defined(__wuwanlib__HttpRequestIOS__) */
