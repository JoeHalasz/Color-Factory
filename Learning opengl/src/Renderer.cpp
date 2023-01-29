#pragma once
#include "Renderer.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>

std::vector<unsigned int> makeIndices(int numQuads)
{
    std::vector<unsigned int> indices((numQuads * 6));

    int pos = 0;
    for (unsigned int i = 0; i < numQuads * 4; i += 4)
    {   // should be 0,1,2,2,3,0,
        indices[pos++] = i;
        indices[pos++] = i + 1;
        indices[pos++] = i + 2;
        indices[pos++] = i + 2;
        indices[pos++] = i + 3;
        indices[pos++] = i;
    }

    return indices;
}

struct Vec3 { float x, y, z; };
struct Vec2 { float x, y; };
struct Vec4 { float x, y, z, w; };

struct Vertex {
    Vec3 position;
    Vec2 TexCoords;
    float TexIndex;
};

static std::array<Vertex, 4> CreateQuad(float x, float y, float size, float textureID) 
{
    
}

Renderer::Renderer()
{

    float positions[] = {
        100.0f, 100.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        100.0f, 200.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        200.0f, 100.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        300.0f, 100.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        300.0f, 200.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        200.0f, 200.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices = makeIndices(2);
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions) * sizeof(float));

    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);
    m_Layout->Push<float>(1);
    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_Layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    m_Shader = std::make_unique<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    m_Shader->Bind();

    m_Textures.push_back(std::make_unique<Texture>("res/textures/floortile.png"));
    m_Textures.push_back(std::make_unique<Texture>("res/textures/floortile2.png"));


}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw() const
{
    m_Shader->Bind();
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::OnRender()
{
    glm::vec3 translationA(200, 100, 0);
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    Clear();

    for (int i = 0; i < m_Textures.size(); i++) {
        m_Textures[i]->Bind(i);
    }
    int samplers[2] = { 0, 1 };
    m_Shader->SetUniform1iv("u_Textures", 2, samplers);
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
        glm::mat4 mvp = proj * view * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        Draw();
    }
}

