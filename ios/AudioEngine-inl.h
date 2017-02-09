#ifndef __AUDIO_ENGINE_INL_H_
#define __AUDIO_ENGINE_INL_H_

#include <unordered_map>
#include "AudioCache.h"
#include "AudioPlayer.h"

#define MAX_AUDIOINSTANCES 100//32

class AudioEngineThreadPool;

class AudioEngineImpl
{
public:
    AudioEngineImpl();
    ~AudioEngineImpl();
    
    bool init();
    int play2d(std::string &fileFullPath ,bool loop ,float volume);
    void setVolume(int audioID,float volume);
    void setLoop(int audioID, bool loop);
    bool pause(int audioID);
    bool resume(int audioID);
    bool stop(int audioID);
    void stopAll();
    float getDuration(int audioID);
    float getCurrentTime(int audioID);
    bool setCurrentTime(int audioID, float time);
    //void setFinishCallback(int audioID, const std::function<void (int, const std::string &)> &callback);
    
    void uncache(const std::string& filePath);
    void uncache(int audioID);
    void uncacheAll();
    
    void update();
    
private:
    void removeAudioID(size_t audioID);
    void removeCache(AudioCache* cache);
    
    void _play2d(AudioCache *cache, int audioID);
    
    AudioEngineThreadPool* _threadPool;
    
    ALuint _alSources[MAX_AUDIOINSTANCES];
    
    //source,used
    std::unordered_map<ALuint, bool> _alSourceUsed;
    
    //filePath,bufferInfo
    std::unordered_map<std::string, AudioCache> _audioCaches;
    
    //audioID,AudioInfo
    std::unordered_map<int, AudioPlayer>  _audioPlayers;
    
    std::mutex _threadMutex;
    
    std::vector<AudioCache*> _toRemoveCaches;
    std::vector<int> _toRemoveAudioIDs;
    
    int _currentAudioID;
    
};
#endif // __AUDIO_ENGINE_INL_H_
