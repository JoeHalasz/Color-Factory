#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "MathStuff.h"


class PaintBlob
{
private:
	glm::vec3 m_Pos;
	glm::vec3 m_Speed = glm::vec3(0.0f);
	
	
	glm::vec4 m_Color;

public:
	PaintBlob(glm::vec3 pos, glm::vec4 color);
	PaintBlob(glm::vec3 pos, glm::vec4 color, glm::vec3 speed);

	void CombineColor(glm::vec4 otherColor);
	glm::vec4 ConvertToRGB();

	inline void SetSpeed(glm::vec3 speed) { m_Speed = speed; }
	inline void SetColor(glm::vec4 color) { m_Color = color; }

	inline glm::vec3 GetPos() const { return m_Pos; }
	inline glm::vec3 GetSpeed() const { return m_Speed; }

};