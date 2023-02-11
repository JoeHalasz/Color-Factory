#pragma once

#include <algorithm>
#include <iostream>



struct Vec3 
{ float x, y, z; 
    inline Vec3 operator*(float a) {
        return { x*a,y*a };
    }
    inline Vec3 operator+(float a) {
        return { x + a,y + a };
    }
    inline Vec3 operator+(Vec3 a) {
        return { x + a.x,y + a.y };
    }
    inline void operator=(Vec3 a) {
        { x = a.x; y = a.y; z = a.z; };
    }
    inline bool operator==(Vec3 a) {
        { return x == a.x && y == a.y && z == a.z; };
    }
};
struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };

struct Vertex {
    Vec3 Position;
    Vec2 TexCoords;
    float TexIndex;
    Vec4 BlobColor;
};

inline const float checkHowClose(const float x, const float y)
{
    return std::abs(x - y);
}

inline const float checkHowClose(const Vec3 x, const Vec3 y)
{
    return std::max(std::abs(x.x - y.x), std::abs(x.y - y.y));
}


Vec3 roundVec3(Vec3 var);