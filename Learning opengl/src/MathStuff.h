#pragma once

struct Vec3 { float x, y, z; };
struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };

struct Vertex {
    Vec3 Position;
    Vec2 TexCoords;
    float TexIndex;
    Vec4 BlobColor;
};