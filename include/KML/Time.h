#ifndef KML_TIME_H
#define KML_TIME_H

namespace KML {
    class Clock {
      public:
        Clock();
        ~Clock();
        double Tick() const;
        double Now() const;

        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
        Clock(Clock&&) = delete;
        Clock& operator=(Clock&&) = delete;
      private:
        struct IMP;
        IMP* imp;
    };
    class Stopwatch {
      public:
        Stopwatch(Clock& __c);
        void Start();
        double Stop(); //Returns the time it took between start & stop, needs to call Start() to reset
        double Query() const; //Returns the time that passed since start (no reset)
      private:
        Clock& _clock;
        double _start;
        bool _running;
    };
}

#endif