#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

class World
{
private:
	GLFWwindow* m_Window;
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Speed = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_ZoomAmount = .25f;
	float m_ZoomChange = 0;
	float m_Rotation = 0;
	float m_RotationChange = 0;
	

	float ZOOM_MAX = 3.0f;
	float ZOOM_MIN = .01f;
	float ZOOM_CHANGE_BY = .02f;
	float ORIGINAL_MOVE_SPEED = 5.0f;
	float ROTATION_SPEED = .25f;
public:
	bool IS3D = false;
	
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	World(GLFWwindow* window, glm::vec3 position, glm::vec3 speed);
	~World();
	bool KeyPressed(int key);
	void checkKeyPresses();
	void OnUpdate();
	
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::vec3 GetSpeed() const { return m_Speed; }
	inline float GetZoomAmount() const { return m_ZoomAmount;  }
	inline float GetRotation() const { return m_Rotation;  }


};