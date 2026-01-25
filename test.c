// Enable Vorbis support (using stb_vorbis)
#define STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
// After miniaudio, include the stb_vorbis implementation
#undef STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"

#include <stdio.h>
#include <unistd.h>

int main() {
    ma_engine engine;
    ma_engine_init(NULL, &engine);

    ma_sound sound;
    ma_result r = ma_sound_init_from_file(&engine, "test_safe.ogg", MA_SOUND_FLAG_STREAM, NULL, NULL, &sound);

    if(r != MA_SUCCESS) printf("OGG load failed: %d (%s)\n", r, ma_result_description(r));
    else printf("OGG loaded!\n");

    ma_sound_start(&sound);

    sleep(5);

    ma_engine_uninit(&engine);
    return 0;
}
