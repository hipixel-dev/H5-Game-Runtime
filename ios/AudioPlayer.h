#ifndef __AUDIO_PLAYER_H_
#define __AUDIO_PLAYER_H_

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#import <OpenAL/al.h>


class AudioCache;
class AudioEngineImpl;

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();
    
    //queue buffer related stuff
    bool setTime(float time);
    float getTime() { return _currTime;}
    bool setLoop(bool loop);
    
protected:
    void rotateBufferThread(int offsetFrame);
    bool play2d(AudioCache* cache);
    
    AudioCache* _audioCache;
    
    float _volume;
    bool _loop;    
    bool _ready;
    ALuint _alSource;
    
    //play by circular buffer
    float _currTime;
    bool _streamingSource;
    ALuint _bufferIds[3];
    std::thread _rotateBufferThread;
    std::condition_variable _sleepCondition;
    std::mutex _sleepMutex;
    bool _exitThread;
    bool _timeDirty;
    
    friend class AudioEngineImpl;
};
#endif // __AUDIO_PLAYER_H_
