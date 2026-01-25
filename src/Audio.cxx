#include <miniaudio/miniaudio.h>

#include <fmt/core.h>

#include <unordered_map>
#include <string>

#include "KML/Audio.h"
#include "__KML/graphics.h"

#include <sys/stat.h>

bool fileExists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

std::unordered_map<std::string, ma_sound*> sounds;

ma_engine engine;


ma_sound* getSound(const char* f) {
    if(sounds.find(f) == sounds.end()) return nullptr;
    return sounds.at(f);
}

void __KML::InitAudioDevice() {
    ma_engine_init(NULL, &engine);
}

void __KML::CloseAudioDevice() {
    for(auto& i : sounds) {
        ma_sound_stop(i.second);
        ma_sound_uninit(i.second);
        delete i.second;
    }
    sounds.clear();
    ma_engine_uninit(&engine);
}

void KML::LoadSound(const char* file, bool streaming, bool loop) {
    fmt::println("[DEBUG] Loading sound: {}", file);

    if(!fileExists(file)) {
        fmt::println("[DEBUG] File not found: {}", file);
        return;
    }

    if(sounds.find(file) != sounds.end()) return;

    ma_sound* sound = new ma_sound;

    ma_uint32 flags = 0;
    if (streaming) flags |= MA_SOUND_FLAG_STREAM;
    if (loop) flags |= MA_SOUND_FLAG_LOOPING;

    ma_result result = ma_sound_init_from_file(&engine, file, flags, NULL, NULL, sound);

    if(result != MA_SUCCESS) {
        delete sound;
        fmt::print("[ERROR] Failed to load sound: {} (Error code: {})\n", file, ma_result_description(result));
        return;
    }

    sounds[file] = sound;
    fmt::println("[DEBUG] LOADED {}", file);
}

void KML::PlaySound(const char* file) {
    ma_sound* s = getSound(file);
    if(!s) {
        fmt::println("[DEBUG] Tried to play sound not loaded: {}", file);
        return;
    }
    ma_sound_start(s);
}

void KML::StopSound(const char* file) {
    ma_sound* s = getSound(file);
    assert(s);
    ma_sound_stop(s);
}

void KML::UnloadSound(const char* file) {
    ma_sound* s = getSound(file);
    assert(s);
    ma_sound_uninit(s);   
    delete s;
    sounds.erase(file); 
}

void KML::StopAllSounds() {
    for(auto& i : sounds) ma_sound_stop(i.second);
}