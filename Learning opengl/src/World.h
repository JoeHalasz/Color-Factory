#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h> 

class World
{
private:
	GLFWwindow* m_Window;
	glm::vec3 m_Position;
	glm::vec3 m_Speed;
	float m_ZoomAmount;
	float m_ZoomChange;

	float ZOOM_MAX = 1.0f;
	float ZOOM_MIN = .01f;
	float ZOOM_CHANGE_BY = .01f;
	
public:
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	World(GLFWwindow* window, glm::vec3 position, glm::vec3 speed);
	~World();
	void checkKeyPresses();
	void OnUpdate();
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::vec3 GetSpeed() const { return m_Speed; }
	inline float GetZoomAmount() const { return m_ZoomAmount;  }


};