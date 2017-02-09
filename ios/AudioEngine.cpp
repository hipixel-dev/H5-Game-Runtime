#include "AudioEngine.h"
#include "AudioEngine-inl.h"
#include "FileIOIOS.h"
#include "Macros.h"
#include <sys/time.h>
#include <stdio.h>

#define TIME_DELAY_PRECISION 0.0001

#ifdef ERROR
#undef ERROR
#endif // ERROR

const int AudioEngine::INVALID_AUDIO_ID = -1;
const float AudioEngine::TIME_UNKNOWN = -1.0f;

//audio file path,audio IDs
std::unordered_map<std::string,std::list<int>> AudioEngine::_audioPathIDMap;
unsigned int AudioEngine::_maxInstances = MAX_AUDIOINSTANCES;
std::unordered_map<int, AudioEngine::AudioInfo> AudioEngine::_audioIDInfoMap;
std::unordered_map<int, AudioIOS*> AudioEngine::_audioIDIntMap;
AudioEngineImpl* AudioEngine::_audioEngineImpl = nullptr;

void AudioEngine::end()
{
    delete _audioEngineImpl;
    _audioEngineImpl = nullptr;
}

bool AudioEngine::lazyInit()
{
    if (_audioEngineImpl == nullptr)
    {
        _audioEngineImpl = new (std::nothrow) AudioEngineImpl();
        if(!_audioEngineImpl ||  !_audioEngineImpl->init() ){
            delete _audioEngineImpl;
            _audioEngineImpl = nullptr;
           return false;
        }
    }

    return true;
}

static double getTime()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

int AudioEngine::play2d(AudioIOS* interface, bool loop, float volume)
{
    assert(interface);

    int retID = play2d(interface->getFilepath(), loop, volume);
    if (retID != AudioEngine::INVALID_AUDIO_ID) {
        _audioIDIntMap[retID] = interface;
    }
    
    return retID;
}

int AudioEngine::play2d(std::string& filePath, bool loop, float volume)
{
    int ret = AudioEngine::INVALID_AUDIO_ID;

    do {
        if ( !lazyInit() ){
            break;
        }
        
        if (_audioIDInfoMap.size() >= _maxInstances) {
            char outStr[300] = {0};
            sprintf(outStr, "Fail to play %s cause by limited max instance of AudioEngine\n",filePath.c_str());
            OutputDebugStringA(outStr);
            break;
        }
        
        if (volume < 0.0f) {
            volume = 0.0f;
        }
        else if (volume > 1.0f){
            volume = 1.0f;
        }
        
        ret = _audioEngineImpl->play2d(filePath, loop, volume);
        if (ret != INVALID_AUDIO_ID)
        {
            _audioPathIDMap[filePath].push_back(ret);
            auto it = _audioPathIDMap.find(filePath);
            
            auto& audioRef = _audioIDInfoMap[ret];
            audioRef.volume = volume;
            audioRef.loop = loop;
            audioRef.is3dAudio = false;
            audioRef.filePath = &it->first;
        }
    } while (0);

    return ret;
}

void AudioEngine::setLoop(int audioID, bool loop)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end() && it->second.loop != loop){
        _audioEngineImpl->setLoop(audioID, loop);
        it->second.loop = loop;
    }
}

void AudioEngine::setVolume(int audioID, float volume)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end()){
        if (volume < 0.0f) {
            volume = 0.0f;
        }
        else if (volume > 1.0f){
            volume = 1.0f;
        }

        if (it->second.volume != volume){
            _audioEngineImpl->setVolume(audioID, volume);
            it->second.volume = volume;
        }
    }
}

void AudioEngine::pause(int audioID)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end() && it->second.state == AudioState::PLAYING){
        _audioEngineImpl->pause(audioID);
        it->second.state = AudioState::PAUSED;
    }
}

void AudioEngine::pauseAll()
{
    auto itEnd = _audioIDInfoMap.end();
    for (auto it = _audioIDInfoMap.begin(); it != itEnd; ++it)
    {
        if (it->second.state == AudioState::PLAYING)
        {
            _audioEngineImpl->pause(it->first);
            it->second.state = AudioState::PAUSED;
        }
    }
}

void AudioEngine::resume(int audioID)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end() /* && it->second.state == AudioState::PAUSED */){
        _audioEngineImpl->resume(audioID);
        it->second.state = AudioState::PLAYING;
    }
}

void AudioEngine::resumeAll()
{
    auto itEnd = _audioIDInfoMap.end();
    for (auto it = _audioIDInfoMap.begin(); it != itEnd; ++it)
    {
        //if (it->second.state == AudioState::PAUSED)
        {
            _audioEngineImpl->resume(it->first);
            it->second.state = AudioState::PLAYING;
        }
    }
}

void AudioEngine::stop(int audioID)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end()){
        _audioEngineImpl->stop(audioID);

        remove(audioID);
    }
}

void AudioEngine::remove(int audioID)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end()){
        _audioPathIDMap[*it->second.filePath].remove(audioID);
        _audioIDInfoMap.erase(audioID);
        
        _audioIDIntMap[audioID]->released();
        _audioIDIntMap.erase(audioID);
    }
}

void AudioEngine::stopAll()
{
    if(!_audioEngineImpl){
        return;
    }
    _audioEngineImpl->stopAll();
    
    _audioPathIDMap.clear();
    _audioIDInfoMap.clear();
}

void AudioEngine::uncache(int audioID)
{
    auto itor = _audioIDIntMap.find(audioID);
    if (itor != _audioIDIntMap.end()) {
        uncache(itor->second->getFilepath());
    }
}

void AudioEngine::uncache(const std::string &filePath)
{
    if(_audioPathIDMap.find(filePath) != _audioPathIDMap.end()){
        auto itEnd = _audioPathIDMap[filePath].end();
        for (auto it = _audioPathIDMap[filePath].begin() ; it != itEnd; ++it) {
            auto audioID = *it;
            _audioEngineImpl->stop(audioID);
        }
        _audioPathIDMap.erase(filePath);
    }

    if (_audioEngineImpl){
        _audioEngineImpl->uncache(filePath);
    }
}

void AudioEngine::uncacheAll()
{
    if(!_audioEngineImpl){
        return;
    }
    stopAll();
    _audioEngineImpl->uncacheAll();
}

float AudioEngine::getDuration(int audioID)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end() && it->second.state != AudioState::INITIALZING)
    {
        if (it->second.duration == TIME_UNKNOWN)
        {
            it->second.duration = _audioEngineImpl->getDuration(audioID);
        }
        return it->second.duration;
    }
    
    return TIME_UNKNOWN;
}

bool AudioEngine::setCurrentTime(int audioID, float time)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end() && it->second.state != AudioState::INITIALZING){
        return _audioEngineImpl->setCurrentTime(audioID, time);
    }

    return false;
}

float AudioEngine::getCurrentTime(int audioID)
{
    auto it = _audioIDInfoMap.find(audioID);
    if (it != _audioIDInfoMap.end() && it->second.state != AudioState::INITIALZING){
        return _audioEngineImpl->getCurrentTime(audioID);
    }
    return 0.0f;
}

bool AudioEngine::setMaxAudioInstance(int maxInstances)
{
    if (maxInstances > 0 && maxInstances <= MAX_AUDIOINSTANCES) {
        _maxInstances = maxInstances;
        return true;
    }

    return false;
}

bool AudioEngine::isLoop(int audioID)
{
    auto tmpIterator = _audioIDInfoMap.find(audioID);
    if (tmpIterator != _audioIDInfoMap.end())
    {
        return tmpIterator->second.loop;
    }
    
    //log("AudioEngine::isLoop-->The audio instance %d is non-existent", audioID);
    return false;
}

float AudioEngine::getVolume(int audioID)
{
    auto tmpIterator = _audioIDInfoMap.find(audioID);
    if (tmpIterator != _audioIDInfoMap.end())
    {
        return tmpIterator->second.volume;
    }

    //log("AudioEngine::getVolume-->The audio instance %d is non-existent", audioID);
    return 0.0f;
}

AudioEngine::AudioState AudioEngine::getState(int audioID)
{
    auto tmpIterator = _audioIDInfoMap.find(audioID);
    if (tmpIterator != _audioIDInfoMap.end())
    {
        return tmpIterator->second.state;
    }
    
    return AudioState::ERROR;
}

void AudioEngine::update()
{
    if ( lazyInit() ){
        _audioEngineImpl->update();
    }
}