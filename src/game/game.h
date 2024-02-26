
#pragma once

#include <sys/sys_types.h>

#include "enums.h"
#include "structs.h"
#include "utils/hooking.h"

#define PI 3.14159265358979323846

#pragma diag_suppress 1070
template<typename T>
class symbol
{
public:
    constexpr symbol(uintptr_t obj)
        : mp_object_(obj)
    { }

    operator T* ()
    {
        volatile uintptr_t object_fptr[]{ mp_object_, hook::toc() };

        T* type = *reinterpret_cast<T*>(mp_object_);
        return (decltype(type))object_fptr;
    }

    T* get()
    {
        return mp_object_;
    }

    uintptr_t mp_object_;
};

float cos(float x);
float sin(float x);
float CalculateTimeFraction(int startTime, int duration);

template<typename T>
static T Lerp(T from, T to, float fraction)
{
    return from + (to - from) * fraction;
}

template<typename T>
static T Clamp(T value, T min, T max)
{
    return value < min ? min : value > max ? max : value;
}

template<typename T>
static T Min(T a, T b)
{
    return a < b ? a : b;
}

template<typename T>
static T Max(T a, T b)
{
    return a > b ? a : b;
}

#include "symbols.h"