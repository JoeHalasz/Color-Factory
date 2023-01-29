#include "RenderLots.h"

#include <array>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//RenderLots::RenderLots(Shader shader, Renderer renderer)
//	: m_Shader(shader), m_Renderer(renderer)
//{
//	
//}

RenderLots::~RenderLots()
{

}

static std::array<Vertex, 4> createVerticiesForQuad(float x, float y, float ID)
{

	float size = 1.0f;

	Vertex v0{};
	v0.position = { x, y, 0.0f };
	v0.TexCoords = { 0.0f, 0.0f };
	v0.TexID = ID;

	Vertex v1{};
	v0.position = { x + size, y, 0.0f };
	v0.TexCoords = { 1.0f, 0.0f };
	v0.TexID = ID;

	Vertex v2{};
	v0.position = { x + size, y + size, 0.0f };
	v0.TexCoords = { 1.0f, 1.0f };
	v0.TexID = ID;

	Vertex v3{};
	v0.position = { x, y + size, 0.0f };
	v0.TexCoords = { 0.0f, 1.0f };
	v0.TexID = ID;

	return { v0, v1, v2, v3 };
}

static std::array<unsigned int, 6> createIndeciesForQuad(unsigned int start)
{
	return { start, start+1, start+2, start+2, start+3, start };
}

bool RenderLots::RenderAsQuad(float xPos, float yPos,float TexID)
{
	

	std::array<Vertex, 4> quadVertices = createVerticiesForQuad(xPos, yPos, m_RenderCount);
	memcpy(m_Vertices, quadVertices.data() + (m_RenderCount * quadVertices.size()), quadVertices.size() * sizeof(Vertex));
	
	std::array<unsigned int, 6> quadIndicies = createIndeciesForQuad(m_RenderCount);
	memcpy(m_Indices, quadIndicies.data() + (m_RenderCount * quadIndicies.size()), quadIndicies.size() * sizeof(int));

	m_IDs[m_RenderCount] = TexID;

	m_RenderCount++;
	return true;
}

float RenderLots::SetTexture(std::string filePath) 
{
	Texture texture(filePath);
	m_Textures.push_back(texture);
	return m_Textures.size() - 1;
}



void RenderLots::Draw()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		// if it hasnt been bound yet then bind it
		m_Shader.Bind();
		m_Textures[i].Bind(i);
		m_Shader.SetUniform1i("u_Texture", i);
		

		if (i >= 32)
		{
			std::cout << "TOO MANY TEXTURES" << std::endl;
		}

		
	}
	VertexArray va;
	VertexBuffer vb(m_Vertices, sizeof(Vertex) * m_RenderCount * 4);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(1);
	
	va.AddBuffer(vb, layout);
	IndexBuffer ib(m_Indices, m_RenderCount * 6);
	m_Shader.Bind();

	m_Shader.SetUniform1i("u_Texture", 0);

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::vec3 translationA(200, 100, 0);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
	glm::mat4 mvp = proj * view * model;
	m_Shader.SetUniformMat4f("u_MVP", mvp);

	// m_Renderer.Draw(va, ib, m_Shader);
	// finished copying the code. Now make this run once and test it 
	
}
