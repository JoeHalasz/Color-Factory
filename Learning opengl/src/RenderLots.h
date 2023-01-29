#pragma once
#include "ErrorCatching.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include <vector>


struct Vec3
{
	float x, y, z;
};

struct Vec2
{
	float x, y; 
};
struct Vec4
{
	float x, y, z, w;
};

struct Vertex
{
	Vec3 position;
	Vec2 TexCoords;
	float TexID;
};


class RenderLots
{
private:
	float m_Vertices[1000*4];
	unsigned int m_Indices[1000*6];
	float m_IDs[1000];
	std::vector<Texture> m_Textures;
	Shader m_Shader;
	Renderer m_Renderer;
	unsigned int m_RenderCount;

public:
	RenderLots(Shader shader, Renderer renderer);
	~RenderLots();


	float SetTexture(std::string filePath);
	bool RenderAsQuad(float xPos, float yPos, float TexID);
	void Draw();
};