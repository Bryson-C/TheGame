//
// Created by Owner on 1/11/2023.
//

#ifndef SDL_TIMER_HPP
#define SDL_TIMER_HPP

#include <time.h>

class Timer {
private:
    clock_t time;
public:
    Timer() : time(clock()) {}
    // Returns Whether The Delay Has Been Passed Since Last Call Or Initialization
    inline bool isComplete(int delay) const { return (clock() - time > delay);  }
    // Resets Timer
    inline void reset() { time = clock(); }
    // Returns If The Timer Is Complete,
    //      If It Is Complete, Reset The Timer And Return True,
    //      Otherwise Let The Timer Continue And Return False
    inline bool isCompleteReset(int delay) {
        bool complete = (clock() - time > delay);
        if (complete) reset();
        return complete;
    }
    inline clock_t getTime() {
        return (clock()-time);
    }
};

class TimerSignal {
private:
    clock_t time;
    bool signaled;
public:
    TimerSignal() : time(clock()), signaled(false) {}
    inline bool isComplete(int delay) const { return (clock() - time > delay);  }
    inline void reset() { time = clock(); }
    inline bool isCompleteReset(int delay) {
        bool complete = (clock() - time > delay);
        if (complete) reset();
        return complete;
    }

    inline void signal(bool value) { signaled = value; }
    inline bool getSignal() const { return signaled; }
};

#endif //SDL_TIMER_HPP
