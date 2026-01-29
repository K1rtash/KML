#ifndef KML_AUDIO_H
#define KML_AUDIO_H

#include "ABI.h"
#include <vector>

/**
 * @file    Audio.h
 * @brief   Play sounds
 */

namespace KML {
    /**
     * @brief      Loads a sound from disc
     *
     * @param[in]  file       File (.ogg, .wav, mp3)
     * @param[in]  streaming  For sounds that are to big
     * @param[in]  loop       Should the sound loop
     */
    KML_API void LoadSound(const char* file, bool streaming = false, bool loop = false);

    /**
     * @brief      Unloads a sound from memory
     *
     * @param[in]  file  Sound file
     */
    KML_API void UnloadSound(const char* file);

    /**
     * @brief      Plays a unique sound
     *
     * @param[in]  file    Sound file
     * @param[in]  volume  Volume
     * @param[in]  pitch   Pitch
     */
    KML_API void PlayMusic(const char* file, float volume = 1.0f, float pitch = 1.0f);

    /**
     * @brief      Stops the music
     *
     * @param[in]  file  Sound file
     */
    KML_API void StopMusic(const char* file); 

    /**
     * @brief      Resets the song 
     *
     * @param[in]  file  Sound file
     */
    KML_API void ResetMusic(const char* file);

    /**
     * @brief      Sets the global pitch multiplier
     *
     * @param[in]  pitch  Pitch (0..2)
     */
    KML_API void SetGlobalPitch(float pitch = 1.0f);

    /**
     * @brief      Sets the global volume.
     *
     * @param[in]  volume  Volume
     */
    KML_API void SetGlobalVolume(float volume = 1.0f);


    /**
     * @class      Sound
     * @brief      A sound with its own pitch and volume
     */
    class KML_API Sound {
      public:
        float volume = 1.0f;
        float pitch = 1.0f;
        
        explicit Sound(const char* file, float vol, float ptc);
        ~Sound();

        Sound(const Sound&) = delete;
        Sound& operator=(const Sound&) = delete;

        Sound(Sound&&) noexcept;
        Sound& operator=(Sound&&) noexcept;

        /**
         * @brief      Resumes or plays the sound
         */
        void Play();

        /**
         * @brief      Updates the sound pitch and volume based on the global state
         */
        void Update();

        /**
         * @brief      Resets the sound to the start
         */
        void Reset();

        /**
         * @brief      Pauses the sound, saving its state
         */
        void Stop(); // pausa 

        /**
         * @brief      Determines if the sound is playing
         *
         * @return     True if playing, False otherwise.
         */
        bool IsPlaying() const;
      private:
        struct IMP;
        IMP* imp;
    };

    /**
     * @class      SoundPool
     * @brief      Manages sounds lifetime
     */
    class KML_API SoundPool {
      public:
        SoundPool();
        /**
         * @brief      Updates sounds state based on global state
         */
        void Update();

        /**
         * @brief      Adds the specified sound.
         *
         * @param      sound  Sound
         *
         * @return     Iterator in the vector
         */
        int Add(Sound&& sound);

        /**
         * @brief      Starts to play all sounds
         */
        void Play();

        /**
         * @brief      Stops and removes all sounds
         */
        void Stop();
        std::vector<Sound> pool;
    };
}

#endif