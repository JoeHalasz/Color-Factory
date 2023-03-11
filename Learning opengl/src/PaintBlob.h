#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>
#include "glm/glm.hpp"
#include "WorldTile.h"
#include "GameObject.h"



class PaintBlob : public GameObject
{
private:

	Vec3 m_Speed = { 0,0,0 };

public:
	~PaintBlob();
	PaintBlob(Vec3 pos, int size, Vec4 color=Vec4{0,0,0,1}, Direction direction=DirectionUp);

	bool operator==(const PaintBlob& other);

	inline Vec3 GetSpeed() const { return m_Speed; }
	inline void SetSpeed(Vec3 speed) { m_Speed = speed; }
	inline void SetColor(Vec4 color) { GetTile()->SetColor(color); }

	bool MoveBy(Vec3 amount, std::unordered_map<int, std::unordered_map<int, std::vector<PaintBlob>>>& PaintBlobs);

	Vec4 ConvertToRGB();
};