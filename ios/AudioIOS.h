//
//  AudioIOS.h
//  wuwanlib
//
//  Created by 陈JH on 15/8/14.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#ifndef __wuwanlib__AudioIOS__
#define __wuwanlib__AudioIOS__

#include"../AudioInterface.h"
#include <string>

class AudioIOS: public AudioInterface {
public:
    
    AudioIOS();
    virtual ~AudioIOS();
    
    virtual bool Release() ;
    
    
    virtual bool Open(const char*szFileName);
    virtual bool Play(unsigned long dwFrom, bool bRepeat) ;
    virtual bool Pause() ;
    virtual bool Stop() ;
    virtual bool Seek(unsigned long dwTo);
    virtual unsigned long GetLength();
    virtual unsigned long GetCurrentPosition();
    virtual bool Close() ;
    virtual bool isOpen() ;
    virtual bool isStop() ;
    virtual void update();
    virtual AudioInterface*Create() ;
    // called when audio is released by AudioEngine because of playing to the end.
    void released();
    std::string& getFilepath();
    
protected:
    std::string _filepath;
    int _audioID;
};

#endif /* defined(__wuwanlib__AudioIOS__) */
