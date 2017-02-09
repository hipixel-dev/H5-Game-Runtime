#ifndef __AUDIO_CACHE_H_
#define __AUDIO_CACHE_H_

#import <OpenAL/al.h>
#import <AudioToolbox/AudioToolbox.h>

#include <string>
#include <mutex>
#include <vector>

#define QUEUEBUFFER_NUM 3
#define QUEUEBUFFER_TIME_STEP 0.1

class AudioEngineImpl;
class AudioPlayer;

class AudioCache{
public:
    AudioCache();
    ~AudioCache();

    void addCallbacks(const std::function<void()> &callback);
    
protected:
    void readDataTask();
    void invokingCallbacks();
    
    //pcm data related stuff
    ALsizei _dataSize;
    ALenum _format;
    ALsizei _sampleRate;
    float _duration;
    int _bytesPerFrame;
    AudioStreamBasicDescription outputFormat;
    
    /*Cache related stuff;
     * Cache pcm data when sizeInBytes less than PCMDATA_CACHEMAXSIZE
     */
    ALuint _alBufferId;
    char* _pcmData;
    SInt64 _bytesOfRead;

    /*Queue buffer related stuff
     *  Streaming in openal when sizeInBytes greater then PCMDATA_CACHEMAXSIZE
     */
    char* _queBuffers[QUEUEBUFFER_NUM];
    ALsizei _queBufferSize[QUEUEBUFFER_NUM];
    UInt32 _queBufferFrames;
    UInt32 _queBufferBytes;

    bool _alBufferReady;
    std::mutex _callbackMutex;
    
    std::vector< std::function<void()> > _callbacks;
    std::mutex _readDataTaskMutex;
    
    bool _exitReadDataTask;
    std::string _fileFullPath;
    
    friend class AudioEngineImpl;
    friend class AudioPlayer;
} ;


#endif // __AUDIO_CACHE_H_

