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
}

#endif