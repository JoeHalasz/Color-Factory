#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"


struct Vec3 { float x, y, z; };
struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };

struct Vertex {
    Vec3 Position;
    Vec2 TexCoords;
    float TexIndex;
};

class Renderer
{
private:
    std::vector<std::vector<Vertex>> m_AllQuads;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexBufferLayout> m_Layout;
    std::vector<std::unique_ptr<Texture>> m_Textures;
    unsigned int m_MAXNUMQUADS;
public:
    Renderer();
    void Clear() const;
    void Clean();
    void Draw() const;
    void OnRender(int width, int height, glm::vec3 position, float zoomAmount, float rotation);
    void AddQuad(float textureID, float size, float x, float y, float z = 1.0f);
    inline unsigned int GetAmountOfCurrentQuads() { return m_AllQuads.size(); }
    inline unsigned int GetMaxAmountOfQuads() { return m_MAXNUMQUADS; }
};