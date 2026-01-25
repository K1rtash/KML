#ifndef KML_AUDIO_H
#define KML_AUDIO_H

/**
 * @file    Audio.h
 * @brief   Play sounds
 */

namespace KML {
    void LoadSound(const char* file, bool streaming = false, bool loop = false);
    void PlaySound(const char* file);
    void StopSound(const char* file); 
    void UnloadSound(const char* file);
    void StopAllSounds();
}

#endif