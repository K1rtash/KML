#ifndef KML_AUDIO_H
#define KML_AUDIO_H

#include <vector>

/**
 * @file    Audio.h
 * @brief   Play sounds
 */

namespace KML {
    void LoadSound(const char* file, bool streaming = false, bool loop = false);
    void UnloadSound(const char* file);

    void PlayMusic(const char* file, float volume = 1.0f, float pitch = 1.0f);
    void StopMusic(const char* file); 
    void ResetMusic(const char* file);

    void SetGlobalPitch(float pitch = 1.0f);
    void SetGlobalVolume(float volume = 1.0f);


    class Sound {
      public:
        float volume = 1.0f;
        float pitch = 1.0f;
        
        explicit Sound(const char* file);
        ~Sound();

        Sound(const Sound&) = delete;
        Sound& operator=(const Sound&) = delete;

        Sound(Sound&&) noexcept;
        Sound& operator=(Sound&&) noexcept;

        void Play(); // resume / start sound
        void Update(); // si se quieren hacer efectos
        void Reset(); // reinicia
        void End();
        void Stop(); // pausa 
        bool IsPlaying() const;
      private:
        struct IMP;
        IMP* imp;
    };

    class SoundPool {
      public:
        SoundPool();
        void Update();
        int Add(Sound&& sound);
        void Play();
        void Stop();
        std::vector<Sound> pool;
    };
}

#endif