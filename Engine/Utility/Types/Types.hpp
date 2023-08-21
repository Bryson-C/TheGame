//
// Created by Owner on 3/29/2023.
//

#ifndef SDL_TYPES_HPP
#define SDL_TYPES_HPP

#include <iostream>
#include <concepts>
#include "SDL.h"

// Stolen: https://www.geeksforgeeks.org/program-calculate-distance-two-points/#
inline float distance(int x1, int y1, int x2, int y2) {
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}


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

    v2<int> add(int a) { return { x + a, y + a}; }
    v2<int> add(int a, int b) { return { x + a, y + b}; }
};


template<typename T> concept concept_ConvertableToInteger = requires(T type) {
    { type } -> std::convertible_to<int>;
};
template<typename T> concept concept_ConvertableToSDLRect = requires(T type) {
    { type } -> std::convertible_to<SDL_Rect>;
};

template<typename T>
struct v3 {
    T x, y, z;

    v3() = default;
    v3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    inline void print() { std::cout << x << ", " << y << ", " << z << "\n"; }

    v3 operator+(v3<T> vec) { return {x + vec.x, y + vec.y, z + vec.z}; }
    void operator+=(v3<T> vec) { x += vec.x, y += vec.y; z += vec.z; }

    v3<int> add(int a) { return { x + a, y + a, z + a}; }
    v3<int> add(int a, int b, int c) { return { x + a, y + b, z + c}; }
};



template<typename T>
struct v4 {
    T x, y, z, w;

    v4() = default;
    v4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
    inline explicit operator SDL_Rect() { return {x,y,z,w}; }


    inline void print() { std::cout << x << ", " << y << ", " << z << ", " << w << "\n"; }

    v4 operator+(v4<T> vec) { return {x + vec.x, y + vec.y, z + vec.z, w + vec.w}; }
    void operator+=(v4<T> vec) { x += vec.x, y += vec.y, z += vec.z, w += vec.w; }
};

inline v4<int> rectToV4(SDL_Rect rect) {
    return v4<int>(rect.x, rect.y, rect.w, rect.h);
}

// TODO: Make Watcher Thread And Global Update Function
template<typename T>
struct ModificationWatcher {
    T value;
    bool modified = false;
    void* (callback)(void);
    ModificationWatcher(T val = T()) : value(val) {}

    inline bool hasChanged() { return modified; }
    inline void whenChanged(void* (fn)(void)) { callback() = fn; }
    inline void updateIfChanged() { whenChanged(); }

};

inline void WatchForModifications() {}


#endif //SDL_TYPES_HPP
