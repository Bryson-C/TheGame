//
// Created by Owner on 3/29/2023.
//

#ifndef SDL_TYPES_HPP
#define SDL_TYPES_HPP

#include <iostream>
#include <concepts>
#include "SDL.h"


template<typename T> concept concept_ConvertableToSDLPoint = requires(T type) {
    { type } -> std::convertible_to<SDL_Point>;
};

template<typename T1, typename T2 = T1>
struct v2 {
    T1 x;
    T2 y;

    v2(T1 t1, T2 t2) : x(t1), y(t2) {}
    explicit operator SDL_Point() { return {x,y}; };

    inline void print() { std::cout << x << ", " << y << "\n"; }

    v2 operator+(v2<T1,T2> vec) { return {x + vec.x, y + vec.y}; }
    void operator+=(v2<T1,T2> vec) { x += vec.x, y += vec.y; }
};

template<typename T> concept concept_ConvertableToSDLRect = requires(T type) {
    { type } -> std::convertible_to<SDL_Rect>;
};

template<typename T>
struct v4 {
    T x, y, z, w;

    v4() {}
    inline explicit operator SDL_Rect() { return {x,y,z,w}; }

    inline void print() { std::cout << x << ", " << y << ", " << z << ", " << w << "\n"; }

    v4 operator+(v4<T> vec) { return {x + vec.x, y + vec.y, z + vec.z, w + vec.w}; }
    void operator+=(v4<T> vec) { x += vec.x, y += vec.y, z += vec.z, w += vec.w; }
};

#endif //SDL_TYPES_HPP
