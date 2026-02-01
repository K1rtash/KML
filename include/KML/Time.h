#ifndef KML_TIME_H
#define KML_TIME_H

/**
 * @file       Time.h
 * @brief      Precise time management 
 */

#include "ABI.h"

namespace KML {
    /**
     * @class      Clock
     * @brief      High-precission clock
     */
    class KML_API Clock {
      public:
        Clock();
        ~Clock();
        /**
         * @brief      Elapsed time since the last call to this method
         *
         * @return     Time in seconds
         */
        double Tick() const;

        /**
         * @brief      Elapsed time since this instance was created
         *
         * @return     Time in seconds
         */
        double Now() const;

        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
        Clock(Clock&&) = delete;
        Clock& operator=(Clock&&) = delete;
      private:
        struct IMP;
        IMP* imp;
    };

    /**
     * @class      Stopwatch
     * @brief      Precise timing using a @ref Clock object
     */
    class KML_API Stopwatch {
      public:
        Stopwatch(Clock& __c);

        /**
         * @brief      Starts the stopwatch
         */
        void Start();

        /**
         * @brief      Gets the elapsed time since @ref Start() was called and halts the stopwatch
         * 
         *             Once stopped, the stopwatch must be started again to call this method
         *
         * @return     Time in seconds
         */
        double Stop();

        /**
         * @brief      Elapsed time since @ref Start() was called
         *
         *             Will not modify or stop the stopwatch
         *
         * @return     Time in seconds
         */
        double Query() const;
      private:
        Clock& _clock;
        double _start;
        bool _running;
    };

    class KML_API Timer {
      public:
        /**
         * @brief      Creates a new instance of a timer
         *
         * @param      clock        Clock to use
         * @param[in]  countdown    Time for the countdown (s)
         * @param[in]  cb           Function callback taking user_pointer as args
         * @param[in]  user_pointer Pointer to user defined data
         */
        Timer(Clock& clock, double countdown, void (*cb)(void*), void* user_pointer);

        /**
         * @brief      Starts the countdown
         */
        void Start();

        /**
         * @brief      Prematurely halts the timer and calls the function callback
         *
         * @return     Time left (s)
         */
        double Stop();

        /**
         * @brief      Updates the timer, in case the time is up, halts it and calls the function callback
         *
         * @return     Time left (s)
         */
        double Query();
      private:
        Clock& _clock;
        double _timeTarget;
        bool _running;
        void (*callback)(void*);
        void* usr_ptr; 
        void end();
    };
}

#endif