//
//  AudioIOS.cpp
//  wuwanlib
//
//  Created by 陈JH on 15/8/14.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#include "AudioIOS.h"
#include "AudioEngine.h"
#include "Macros.h"
//#include "CacheManager.h"
#include "../FileIOInterface.h"
#include "../JSVMInterface.h"

AudioIOS::AudioIOS()
: _audioID(AudioEngine::INVALID_AUDIO_ID)
{
    
}

AudioIOS::~AudioIOS()
{

}

bool AudioIOS::Release()
{
    Close();
    
    delete this;
    return true;
}

bool AudioIOS::Open(const char*szFileName)
{
    /*
    std::string fileName = szFileName;
    FILE* file = [[CacheManager getInstance] loadFileFromUrl:fileName];
    //assert(file);
    if (!file) {
        NSLog(@"Can't open audio file:%s", fileName.c_str());
        _filepath = "";
        return false;
    }
    
    _filepath = szFileName;
    return true;
     */
    
    FileIOInterface* fileIO = JSVM::GetInstance()->GetFileIOInterface();
    if (!fileIO->IsFileExist(szFileName)) {
        printf("Can't open audio file:%s", szFileName);
        return false;
    }
    
    _filepath = szFileName;
    return true;
}

bool AudioIOS::Play(unsigned long dwFrom, bool bRepeat)
{
    if (_audioID == AudioEngine::INVALID_AUDIO_ID) {
        _audioID = AudioEngine::play2d(this, bRepeat);
       
        if (_audioID == AudioEngine::INVALID_AUDIO_ID) {
            assert(0);
            return false;
        }
    }
    else
    {
        AudioEngine::resume(_audioID);
    }
    
    return true;
}

bool AudioIOS::Pause()
{
    AudioEngine::pause(_audioID);
    return true;
}

bool AudioIOS::Stop()
{
    AudioEngine::stop(_audioID);
    return true;
}

bool AudioIOS::Seek(unsigned long dwTo)
{
    return AudioEngine::setCurrentTime(_audioID, dwTo);
}

unsigned long AudioIOS::GetLength()
{
    return AudioEngine::getDuration(_audioID);
}

unsigned long AudioIOS::GetCurrentPosition()
{
    return AudioEngine::getCurrentTime(_audioID);
}

bool AudioIOS::Close()
{
    AudioEngine::stop(_audioID);
    return true;
}

bool AudioIOS::isOpen()
{
    return _audioID != AudioEngine::INVALID_AUDIO_ID;
}

bool AudioIOS::isStop()
{
    return AudioEngine::getState(_audioID) != AudioEngine::AudioState::PLAYING;
}

void AudioIOS::update()
{
    AudioEngine::update();
}

AudioInterface* AudioIOS::Create()
{
    return new AudioIOS;
}

void AudioIOS::released()
{
    _filepath = "";
    _audioID = AudioEngine::INVALID_AUDIO_ID;
}

std::string& AudioIOS::getFilepath()
{
    return _filepath;
}