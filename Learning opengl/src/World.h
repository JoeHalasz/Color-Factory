#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "Input.h"

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

public:
	bool IS3D = false; // Only works if there is only one texture
	
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	~World();
	bool KeyPressed(int key);
	void checkKeyPresses();
	void OnUpdate(Input* input);

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline int GetZoomAmount() const { return (int)m_ZoomAmount;  }
	inline float GetRotation() const { return m_Rotation;  }
	inline int GetBlockSize() { return m_BlockSize; }
	inline int GetMousePosX() { return m_MousePosX; }
	inline int GetMousePosY() { return m_MousePosY; }


};