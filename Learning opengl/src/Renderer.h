#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>



class Renderer
{
private:
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexBufferLayout> m_Layout;
    std::unique_ptr<Texture> m_Texture;
public:
    Renderer();
    void Clear() const;
    void Draw() const;
    void OnRender();
};