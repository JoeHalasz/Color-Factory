#include "PaintBlob.h"

PaintBlob::PaintBlob(glm::vec3 pos, glm::vec4 color)
	:m_Pos(pos), m_Color(color)
{
}

PaintBlob::PaintBlob(glm::vec3 pos, glm::vec4 color, glm::vec3 speed)
	:m_Pos(pos), m_Color(color), m_Speed(speed)
{
}

void PaintBlob::CombineColor(glm::vec4 otherColor)
{

}

glm::vec4 PaintBlob::ConvertToRGB()
{
	glm::vec4 RGBValues;
	RGBValues.a = 1;
	RGBValues.r = (1 - m_Color[0]) * (1 - m_Color[3]);
	RGBValues.g = (1 - m_Color[1]) * (1 - m_Color[3]);
	RGBValues.b = (1 - m_Color[2]) * (1 - m_Color[3]);
	return RGBValues;
}


