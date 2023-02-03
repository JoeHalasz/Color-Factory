#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

#include "Input.h"
#include "WorldTile.h"

class World
{
private:
	GLFWwindow* m_Window;
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_ZoomAmount = 0;
	int m_BlockSize = 75;

	float m_Rotation = 0;
	float ZOOM_MAX = 20;
	float ZOOM_MIN = 0;

	int m_MousePosX = 0;
	int m_MousePosY = 0;

	std::vector<WorldTile> m_WorldTiles;


	bool AddBelt(TileType beltColor, glm::vec3 pos);

public:
	bool IS3D = false; // Only works if there is only one texture
	
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	~World();
	void OnUpdate(Input* input);
	bool AddWorldTile(WorldTile tile);

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline int GetZoomAmount() const { return (int)m_ZoomAmount;  }
	inline float GetRotation() const { return m_Rotation;  }
	inline int GetBlockSize() { return m_BlockSize; }
	inline int GetMousePosX() { return m_MousePosX; }
	inline int GetMousePosY() { return m_MousePosY; }
	inline std::vector<WorldTile> GetWorldTiles() { return m_WorldTiles; }


};