#ifndef KSL_CLOCK_H_PUBLIC
#define KSL_CLOCK_H_PUBLIC

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ksl_clock ksl_clock;

ksl_clock* ksl_clock_start();
double ksl_clock_step(ksl_clock* t);
double ksl_clock_abs(ksl_clock* t);
double ksl_clock_now(ksl_clock* t);
void ksl_clock_free(ksl_clock** t);

#ifdef KSL_CLOCK_IMPLEMENTATION

#include <stdlib.h>

void ksl_clock_free(ksl_clock** t) {
    if(!t || !*t) return;
    free(*t);
    *t = NULL;
}

#if defined(WIN32) || defined(_WIN32)
    #include <windows.h>

    struct ksl_clock {
        LARGE_INTEGER freq, start, abs;
    };
    
    ksl_clock* ksl_clock_start() {
        ksl_clock* t = (ksl_clock*)malloc(sizeof(ksl_clock));
        if(!t) return NULL;
        QueryPerformanceFrequency(&t->freq);
        QueryPerformanceCounter(&t->abs);
        t->start = t->abs;
        return t;
    }

    double ksl_clock_step(ksl_clock* t) {
        if(!t) return 0;
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        double dt = (double)(now.QuadPart - t->start.QuadPart) / t->freq.QuadPart;
        t->start = now;
        return dt;
    }

    double ksl_clock_now(ksl_clock* t) {
        if (!t) return 0;
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (double)now.QuadPart / t->freq.QuadPart;
    }

    double ksl_clock_abs(ksl_clock* t) {
        if(!t) return 0;
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (double)(now.QuadPart - t->abs.QuadPart) / t->freq.QuadPart;
    }

#elif defined(__linux__) || defined(__APPLE__)
    #include <sys/time.h>
    struct ksl_clock {
        struct timeval start, abs;
    };

    ksl_clock* ksl_clock_start() {
        ksl_clock* t = (ksl_clock*)malloc(sizeof(ksl_clock));
        gettimeofday(&t->start, NULL);
        t->abs = t->start;
        return t;
    }

    double ksl_clock_step(ksl_clock* t) {
        if(!t) return 0;
        struct timeval now;
        gettimeofday(&now, NULL);
        double dt = (now.tv_sec - t->start.tv_sec) + (now.tv_usec - t->start.tv_usec) / 1000000.0;
        t->start = now;
        return dt;
    }

    double ksl_clock_now(ksl_clock* t) {
        (void)t;
        struct timeval now;
        gettimeofday(&now, NULL);
        return (double)now.tv_sec + (double)now.tv_usec / 1000000.0;
    }
    
    double ksl_clock_abs(ksl_clock* t) {
        if(!t) return 0;
        struct timeval now;
        gettimeofday(&now, NULL);
        double dt = (now.tv_sec - t->abs.tv_sec) + (now.tv_usec - t->abs.tv_usec) / 1000000.0;
        return dt;
    }

#else
#error "Sistema no compatible!!"
#endif

#endif // Implementación

#ifdef __cplusplus
}
#endif

#endif // Header guard