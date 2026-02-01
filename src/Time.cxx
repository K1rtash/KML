#include "KML/Time.h"
#define KSL_CLOCK_IMPLEMENTATION
#include "KSL/ksl_clock.h"

#include <cassert>

using namespace KML;

Stopwatch::Stopwatch(Clock& __c) : _clock{__c} {
    _running = false;
}

void Stopwatch::Start() {
    if(_running) return;
    _start = _clock.Now();
    _running = true;
}

double Stopwatch::Stop() {
    if(!_running) return 0;
    _running = false;
    return _clock.Now() - _start;
}

double Stopwatch::Query() const {
    if(!_running) return 0;
    return _clock.Now() - _start;
}


struct Clock::IMP {
    ksl_clock* clock;
};

Clock::Clock() {
    imp = new IMP;
    imp->clock = ksl_clock_start();
}

double Clock::Now() const {
    return ksl_clock_now(imp->clock);
}

double Clock::Tick() const {
    return ksl_clock_step(imp->clock);
}

Clock::~Clock() {
    ksl_clock_free(&(imp->clock));
    delete imp;
}

Timer::Timer(Clock& c, double t, void (*cb)(void*), void* uptr) : _clock{c} {
    _timeTarget = t;
    callback = cb;
    usr_ptr = uptr;
}

void Timer::Start() {
    if(_running) return;
    _running = true;
    _timeTarget += _clock.Now();
}

double Timer::Query() {
    if(!_running) return 0.0;
    double timeLeft = _timeTarget - _clock.Now();
    if(timeLeft <= 0.0) end();
    return timeLeft; 
}

double Timer::Stop() {
    if(!_running) return 0.0;
    end();
    return _timeTarget - _clock.Now();
} 

void Timer::end() {
    _running = false;
    if(usr_ptr && callback) callback(usr_ptr);
}