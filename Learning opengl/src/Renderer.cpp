#pragma once
#include "Renderer.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <array>

#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>


std::vector<unsigned int> makeIndices(int numQuads)
{
    std::vector<unsigned int> indices((numQuads * 6));

    int pos = 0;
    for (int i = 0; i < numQuads * 4; i += 4)
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

static std::vector<Vertex> CreateQuad(float textureID, float size, float x, float y, float z)
{
    std::vector<Vertex> v(4);

    v[0].Position = { x, y, z };
    v[0].TexCoords = { 0.0f, 0.0f };
    v[0].TexIndex = textureID;

    v[1].Position = { x + size, y, z };
    v[1].TexCoords = { 1.0f, 0.0f };
    v[1].TexIndex = textureID;

    v[2].Position = { x + size, y + size, z };
    v[2].TexCoords = { 1.0f, 1.0f };
    v[2].TexIndex = textureID;

    v[3].Position = { x, y + size, z };
    v[3].TexCoords = { 0.0f, 1.0f };
    v[3].TexIndex = textureID;

    return v;

}

Renderer::Renderer()
{
    m_MAXNUMQUADS = 100000;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * m_MAXNUMQUADS * 4); // max amount of vertices

    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);
    m_Layout->Push<float>(1);
    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_Layout);

    m_Shader = std::make_unique<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    m_Shader->Bind();

    m_Textures.push_back(std::make_unique<Texture>("res/textures/floortile.png"));
    m_Textures.push_back(std::make_unique<Texture>("res/textures/floortile dark.png"));
    m_Textures.push_back(std::make_unique<Texture>("res/textures/conveyor belt.png"));


}

void Renderer::Clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );
}

void Renderer::Draw() const
{
    m_Shader->Bind();
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::AddQuad(float textureID, float size, float x, float y, float z)
{
    m_AllQuads.push_back(CreateQuad(textureID, size, x, y, z));
}

void Renderer::OnRender(int width, int height, World world)
{
    std::vector<unsigned int> indices = makeIndices(m_AllQuads.size());
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    Clear();
    // set dynamic vertex buffer
    std::vector<Vertex> vertices(m_AllQuads.size()*4);
    if (m_MAXNUMQUADS < m_AllQuads.size())
    {
        std::cout << "TOO MANY QUADS" << std::endl;
        std::cout << "Need " << m_AllQuads.size() << " Quads" << std::endl;
        std::cout << "Max is " << m_MAXNUMQUADS << " Quads" << std::endl;
    }
    Vertex* buffer = vertices.data();

    for (unsigned int i = 0; i < m_AllQuads.size(); i++)
    {
        memcpy(buffer + (m_AllQuads[i].size() * i), m_AllQuads[i].data(), m_AllQuads[i].size() * sizeof(Vertex));
    }

    m_VertexBuffer->Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

    float v1 = -1 * (float)(width/2) - ((width/2) * world.GetZoomAmount());
    float v2 = -1 * v1;
    float v3 = -1 * (float)(height/2) - ((height/2) * world.GetZoomAmount());
    float v4 = -1 * v3;

    glm::mat4 proj = glm::ortho(v1, v2, v3, v4, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), world.GetPosition());
    Clear();

    for (unsigned int i = 0; i < m_Textures.size(); i++) {
        m_Textures[i]->Bind(i);
    }
    int samplers[3] = { 0, 1, 2};
    m_Shader->SetUniform1iv("u_Textures", 3, samplers);

    if (world.IS3D) {
        view = glm::rotate(view, glm::radians(world.GetRotation()), glm::vec3(1, 0, 0));
        proj = glm::perspective(glm::radians(90.0f), v2/v4, -1.0f, 1.0f);
    }

    {
        glm::mat4 mvp = proj * view;
        
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        Draw();
    }
}

void Renderer::DeleteQuads()
{
    m_AllQuads.clear();
}

