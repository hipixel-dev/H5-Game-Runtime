#ifndef __AUDIO_ENGINE_H_
#define __AUDIO_ENGINE_H_

#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include "AudioIOS.h"

#ifdef ERROR
#undef ERROR
#endif // ERROR

class AudioEngineImpl;

/**
 * @class AudioEngine
 *
 * @brief Offers a interface to play audio.
 *
 * @note Make sure to call AudioEngine::end() when the audio engine is not needed anymore to release resources.
 * 
 */

class AudioEngine
{
public:
    /** AudioState enum,all possible states of an audio instance.*/
    enum class AudioState
    {
        ERROR  = -1,
        INITIALZING,
        PLAYING,
        PAUSED
    };
    
    static const int INVALID_AUDIO_ID;

    static const float TIME_UNKNOWN;

    static bool lazyInit();

    /**
     * Release objects relating to AudioEngine.
     *
     * @warning It must be called before the application exit.
     */
    static void end();
       
    /** 
     * Play 2d sound.
     *
     * @param filePath The path of an audio file.
     * @param loop Whether audio instance loop or not.
     * @param volume Volume value (range from 0.0 to 1.0).
     * @return An audio ID. It allows you to dynamically change the behavior of an audio instance on the fly.
     *
     * @see `AudioProfile`
     */
    static int play2d(std::string& filePath, bool loop = false, float volume = 1.0f);
    static int play2d(AudioIOS* interface, bool loop = false, float volume = 1.0f);
    
    
    /** 
     * Sets whether an audio instance loop or not.
     *
     * @param audioID An audioID returned by the play2d function.
     * @param loop Whether audio instance loop or not.
     */
    static void setLoop(int audioID, bool loop);

    /** 
     * Checks whether an audio instance is loop.
     *
     * @param audioID An audioID returned by the play2d function.
     * @return Whether or not an audio instance is loop.
     */
    static bool isLoop(int audioID);

    /** 
     * Sets volume for an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     * @param volume Volume value (range from 0.0 to 1.0).
     */
    static void setVolume(int audioID, float volume);

    /** 
     * Gets the volume value of an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     * @return Volume value (range from 0.0 to 1.0).
     */
    static float getVolume(int audioID);

    /** 
     * Pause an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     */
    static void pause(int audioID);

    /** Pause all playing audio instances. */
    static void pauseAll();

    /** 
     * Resume an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     */
    static void resume(int audioID);

    /** Resume all suspended audio instances. */
    static void resumeAll();

    /** 
     * Stop an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     */
    static void stop(int audioID);

    /** Stop all audio instances. */
    static void stopAll();

    /**
     * Sets the current playback position of an audio instance.
     *
     * @param audioID   An audioID returned by the play2d function.
     * @param sec       The offset in seconds from the start to seek to.
     * @return 
     */
    static bool setCurrentTime(int audioID, float sec);

    /** 
     * Gets the current playback position of an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     * @return The current playback position of an audio instance.
     */
    static float getCurrentTime(int audioID);

    /** 
     * Gets the duration of an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     * @return The duration of an audio instance.
     */
    static float getDuration(int audioID);

    /** 
     * Returns the state of an audio instance.
     *
     * @param audioID An audioID returned by the play2d function.
     * @return The status of an audio instance.
     */
    static AudioState getState(int audioID);

    
    /**
     * Gets the maximum number of simultaneous audio instance of AudioEngine.
     */
    static int getMaxAudioInstance() {return _maxInstances;}
    
    /**
     * Sets the maximum number of simultaneous audio instance for AudioEngine.
     *
     * @param maxInstances The maximum number of simultaneous audio instance.
     */
    static bool setMaxAudioInstance(int maxInstances);
    
    /** 
     * Uncache the audio data from internal buffer.
     * AudioEngine cache audio data on ios,mac, and win32 platform.
     *
     * @warning This can lead to stop related audio first.
     * @param filePath Audio file path.
     */
    static void uncache(const std::string& filePath);
    static void uncache(int audioID);
    
    /** 
     * Uncache all audio data from internal buffer.
     *
     * @warning All audio will be stopped first.
     */
    static void uncacheAll();
    
    static void remove(int audioID);
    
    static void update();

protected:
    
    struct AudioInfo
    {
        const std::string* filePath;
        
        float volume;
        bool loop;
        float duration;
        AudioState state;
        
        bool is3dAudio;

        AudioInfo()
            : duration(TIME_UNKNOWN)
            , state(AudioState::INITIALZING)
        {

        }
    };

    //audioID,audioAttribute
    static std::unordered_map<int, AudioInfo> _audioIDInfoMap;
    
    //audio file path,audio IDs
    static std::unordered_map<std::string,std::list<int>> _audioPathIDMap;
    static std::unordered_map<int, AudioIOS*> _audioIDIntMap;
    
    static unsigned int _maxInstances;
    
    static AudioEngineImpl* _audioEngineImpl;
    
    friend class AudioEngineImpl;
};
#endif // __AUDIO_ENGINE_H_