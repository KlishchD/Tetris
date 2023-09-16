#pragma once
#include <vector>

#include "Vector.h"

class ColorLibrary
{
public:
    static const ColorLibrary& Get()
    {
        static ColorLibrary library;
        return library;
    }
    
    Vector3f GetColor(int32_t ColorId) const
    {
        return Colors[ColorId];
    }
    
    int32_t GetRandomColorId() const
    {
        return 2 + rand() % (Colors.size() - 2);
    }
    
private:
    std::vector<Vector3f> Colors;
    
    ColorLibrary()
    {
        Colors = {
            { 1.0f, 1.0f, 1.0f },
            { 0.3f, 0.3f, 0.3f },
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
            { 1.0f, 0.4f, 0.0f }
        };
    }
};
