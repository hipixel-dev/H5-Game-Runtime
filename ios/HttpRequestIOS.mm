//
//  HttpRequestIOS.cpp
//  wuwanlib
//
//  Created by 陈JH on 15/8/17.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#include "HttpRequestIOS.h"
#import <Foundation/Foundation.h>

HttpRequestIOS::HttpRequestIOS()
{
    
}

HttpRequestIOS::~HttpRequestIOS()
{
    
}

int HttpRequestIOS::SendPostOrGetRequest(const char*szURL, const char*szType, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen)
{
    // create request with url
    NSString* urlstring = [NSString stringWithUTF8String:szURL];
    NSURL *url = [NSURL URLWithString:urlstring];
    
    NSMutableURLRequest *nsrequest = [NSMutableURLRequest requestWithURL:url
                                                             cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData
                                                         timeoutInterval:15.0];
    
    // set request type
    NSString* reqType = [NSString stringWithUTF8String:szType];
    [nsrequest setHTTPMethod:reqType];
    
    // set content type
    if (szContentType && strlen(szContentType) > 0) {
        NSString *contentType = [NSString stringWithUTF8String:szContentType];
        [nsrequest setValue:contentType forHTTPHeaderField:@"Content-Type"];
    }
    
    // set referer
    if (szReferer && strlen(szReferer) > 0) {
        NSString *referer = [NSString stringWithUTF8String:szReferer];
        [nsrequest setValue:referer forHTTPHeaderField:@"Referer"];
    }
    
    // set post data
    if (szPostData && nDataLen > 0) {
        NSData *postData = [NSData dataWithBytes:szPostData length:nDataLen];
        [nsrequest setHTTPBody:postData];
    }
    
    // send sync request
    NSError* error = nil;
    NSData* data = [NSURLConnection sendSynchronousRequest:nsrequest returningResponse:nil error:&error];
    if (data == nil) {
        NSLog(@"Send %@ Request failed: %@", reqType, error);
        return 404;
    }
    
    size_t dataLen = [data length];
    pDataRet = new char[dataLen + 1];
    memcpy(pDataRet, [data bytes], dataLen);
    pDataRet[dataLen] = 0;
    nRetLen = dataLen;
    
    //[data release];  // NEED release?
    
    return 200;
}