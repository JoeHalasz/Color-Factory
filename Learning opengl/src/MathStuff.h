#pragma once

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
};
struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };

struct Vertex {
    Vec3 Position;
    Vec2 TexCoords;
    float TexIndex;
    Vec4 BlobColor;
};