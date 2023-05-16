//
// Created by Owner on 5/5/2023.
//

#ifndef SDL_MOMENTUM_HPP
#define SDL_MOMENTUM_HPP

#include "stdint.h"

using MomentumIntegerType = int16_t;
struct Momentum {
    using move_t = MomentumIntegerType;

    Momentum(int32_t _maxMomentum) : maxMomentum(_maxMomentum), currentMomentum(0) {}

    move_t maxMomentum;
    move_t currentMomentum;
    inline void push(move_t force) {
        if (abs(currentMomentum + force) > maxMomentum) currentMomentum = (maxMomentum * ((force < 0) ? -1 : 1));
        else currentMomentum += force;
    }
    // Works Like The `push` Function But Instead Of Adding Values Until The Momentum Hits Its Max, The Force Will Stay Below `dontExceed`s Value
    inline void pushDontExceed(move_t force, move_t dontExceed = 0) {
        move_t exceed = (dontExceed != 0) ? dontExceed : 10;
        if (abs(currentMomentum + force) > exceed)
            currentMomentum = (exceed * ((force < 0) ? -1 : 1));
        else currentMomentum += force;
    }
    // Returns Current Momentum Then Lowers `currentMomentum` By One
    inline MomentumIntegerType applyForce() {
        auto momentum = currentMomentum;
        if (currentMomentum > 0) currentMomentum--;
        else if (currentMomentum < 0) currentMomentum++;
        return momentum;
    }
    // Adds (Or Subtracts) The Current Moment To The `value` Parameter, Then Returns Current Momentum ,Also Lowers `currentMomentum` By One
    inline MomentumIntegerType applyForce(int32_t& value) {
        value += currentMomentum;
        if (currentMomentum > 0) currentMomentum--;
        else if (currentMomentum < 0) currentMomentum++;
        return currentMomentum;
    }
    // Adds (Or Subtracts) The Current Moment To The `value` Parameter, Then Returns Current Momentum, Also Lowers `currentMomentum` By One
    inline MomentumIntegerType applyForce(float& value) {
        value += (float)currentMomentum;
        if (currentMomentum > 0) currentMomentum--;
        else if (currentMomentum < 0) currentMomentum++;
        return currentMomentum;
    }
};

#endif //SDL_MOMENTUM_HPP
