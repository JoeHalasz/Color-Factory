#pragma once
#include "Renderer.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



Renderer::Renderer()
{

    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f,
        400.0f, 100.0f, 1.0f, 0.0f,
        400.0f, 400.0f, 1.0f, 1.0f,
        100.0f, 400.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    
    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(2);
    m_Layout->Push<float>(2);
    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_Layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    m_Shader = std::make_unique<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    m_Shader->Bind();

    m_Texture = std::make_unique<Texture>("res/textures/floor tile.png");
    m_Texture->Bind();
    m_Shader->SetUniform1i("u_Texture", 0);

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
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
        glm::mat4 mvp = proj * view * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        Draw();
    }
}

