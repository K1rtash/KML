#include "KML/Time.h"
#define KSL_CLOCK_IMPLEMENTATION
#include "ksl/ksl_clock.h"

using namespace KML;

Stopwatch::Stopwatch(Clock& __c) : _clock{__c} {
    _running = false;
}

void Stopwatch::Start() {
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