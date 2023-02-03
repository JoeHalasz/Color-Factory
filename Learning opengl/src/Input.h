#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

class Input
{
private:
	
	glm::vec3 m_Speed = glm::vec3(0.0f, 0.0f, 0.0f);
	float ORIGINAL_MOVE_SPEED = 5.0f;
	float ZOOM_CHANGE_BY = .02f;
	float ROTATION_SPEED = .25f;

	float m_RotationChange = 0.0f;
	bool changePos = false;
	glm::vec3 m_NewPos = glm::vec3(0.0f);

	int m_LastNumPressed = 1;

	double m_MousePosX = 0;
	double m_MousePosY = 0;

	double m_ScrollOffset = 0;

	bool m_MouseDown = false;
	bool m_MouseUp = false;

	
public:
	GLFWwindow* m_Window;
	bool IS3D = false;
	Input() {};
	Input(GLFWwindow* window, bool IS3D);
	bool KeyPressed(int key);
	bool KeyHeld(int key);
	void CheckInputPresses();
	void Reset();
	inline bool NeedChangePos() { return changePos; }
	inline glm::vec3 GetSpeed() { return m_Speed; }
	inline float GetRotationChange() { return m_RotationChange; }
	inline float GetChangeZoom() { return m_ScrollOffset; }
	inline double GetMousePosX() { return m_MousePosX; }
	inline double GetMousePosY() { return m_MousePosY; }
	inline bool GetMouseUp() { return m_MouseUp; }
	inline bool GetMouseDown() { return m_MouseDown; }
	inline int GetLastNumPressed() { return m_LastNumPressed; }
	void SetMouseXandY(double x, double y);
	void SetScrollOffset(double offset);
};