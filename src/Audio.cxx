#include <miniaudio/miniaudio.h>

#include <unordered_map>
#include <string>

#include "KML/Audio.h"
#include "KML/Utils.h"

#include "__KML/graphics.h"

std::unordered_map<std::string, ma_sound*> sounds;

ma_engine engine;
float g_pitch_mod = 1.0f;

ma_sound* getSound(const char* f) {
    if(sounds.find(f) == sounds.end()) {
        return nullptr;
    }
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
    if(sounds.find(file) != sounds.end()) return;

    ma_sound* sound = new ma_sound;

    ma_uint32 flags = 0;
    if (streaming) flags |= MA_SOUND_FLAG_STREAM;
    if (loop) flags |= MA_SOUND_FLAG_LOOPING;

    ma_result result = ma_sound_init_from_file(&engine, file, flags, NULL, NULL, sound);

    if(result != MA_SUCCESS) {
        delete sound;
        #ifdef KML_PRINT_ERRORS
        printf("Failed to load sound: %s (Error code: %s)\n", file, ma_result_description(result));
        #endif
        return;
    }

    sounds[file] = sound;
}

void KML::SetGlobalVolume(float v) {
    ma_engine_set_volume(&engine, Clamp<float>(v, 0.0f, 1.0f));
}

void KML::SetGlobalPitch(float p) {
    g_pitch_mod = KML::Clamp<float>(p, 0.0f, 2.0f);
}

void KML::PlayMusic(const char* file, float volume, float pitch) {
    ma_sound* s = getSound(file);
    if(!s) return;
    ma_sound_start(s);
    ma_sound_set_pitch(s, KML::Clamp<float>(g_pitch_mod * pitch, 0.0f, 2.0f));
    ma_sound_set_volume(s, KML::Clamp<float>(volume, 0.0f, 1.0f));
}

void KML::StopMusic(const char* file) {
    ma_sound* s = getSound(file);
    if(!s) return;
    ma_sound_stop(s);
}

void KML::ResetMusic(const char* file) {
    ma_sound* s = getSound(file);
    if(!s) return;
    ma_sound_seek_to_pcm_frame(s, 0);
}

void KML::UnloadSound(const char* file) {
    ma_sound* s = getSound(file);
    assert(s);
    ma_sound_uninit(s);   
    delete s;
    sounds.erase(file); 
}

struct KML::Sound::IMP {
    ma_sound instance;
};

KML::Sound::Sound(const char* file, float v, float p) : volume{v}, pitch{p} {
    imp = new IMP{};
    ma_sound_init_copy(&engine, getSound(file), 0, nullptr, &imp->instance);
}

KML::Sound::~Sound() {
    if(imp) {
        if(__KML::contextActive && imp) ma_sound_uninit(&imp->instance);
        delete imp;
        imp = nullptr;
    }
}

KML::Sound::Sound(Sound&& other) noexcept {
    imp = other.imp;
    other.imp = nullptr;
}

KML::Sound& KML::Sound::operator=(Sound&& other) noexcept {
    if (this != &other) {
        if (imp) {
            ma_sound_uninit(&imp->instance);
            delete imp;
        }
        imp = other.imp;
        other.imp = nullptr;
    }
    return *this;
}

void KML::Sound::Update() {
    if(!imp) return;
    ma_sound_set_pitch(&imp->instance, KML::Clamp<float>(pitch * g_pitch_mod, 0.0f, 2.0f));
    ma_sound_set_volume(&imp->instance, KML::Clamp<float>(volume, 0.0f, 1.0f));
}

void KML::Sound::Stop() {
    if(!imp) return;
    ma_sound_stop(&imp->instance);
}

void KML::Sound::Reset() {
    if(!imp) return;
    ma_sound_seek_to_pcm_frame(&imp->instance, 0);
}

void KML::Sound::Play() {
    if(!imp) return;
    ma_sound_start(&imp->instance);
}

bool KML::Sound::IsPlaying() const {
    return imp && ma_sound_is_playing(&imp->instance);
}


KML::SoundPool::SoundPool() {

}

int KML::SoundPool::Add(Sound&& sound) {
    pool.emplace_back(std::move(sound));
    int i = pool.size() - 1;
    pool[i].Play();
    return i;
}

void KML::SoundPool::Update() {
    pool.erase(
        std::remove_if(pool.begin(), pool.end(),
        [](const Sound& s){ return s.IsPlaying() == false; }), pool.end()
    );

    for (auto& s : pool) {
        if (s.IsPlaying()) s.Update();
    }
}

void KML::SoundPool::Stop() {
    for (auto& s : pool) if (s.IsPlaying()) s.Stop();
    pool.clear();
}