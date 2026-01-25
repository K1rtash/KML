// Enable Vorbis support (using stb_vorbis)
#define STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
// After miniaudio, include the stb_vorbis implementation
#undef STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"