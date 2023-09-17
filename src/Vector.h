#pragma once
#include <cstdint>

struct Vector3f
{
    float x;
    float y;
    float z;
    
    Vector3f() = default;
    Vector3f(float X, float Y, float Z): x(X), y(Y), z(Z)
    {
        
    }
};

struct Vector2f
{
    float x;
    float y;
    
    Vector2f(float X, float Y): x(X), y(Y)
    {
    }
};

struct Vector2i
{
    int32_t x;
    int32_t y;
};

