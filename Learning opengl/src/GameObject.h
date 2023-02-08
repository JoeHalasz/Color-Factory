#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "MathStuff.h"
#include "WorldTile.h"
#include <iostream>

enum Direction
{
	DirectionUp = 0,
	DirectionRight = 1,
	DirectionDown = 2,
	DirectionLeft = 3,
};


class GameObject
{
private:
	Vec3 m_WorldPos = {0,0,0};
	int m_Size;
	Direction m_Direction = DirectionUp;
	WorldTile m_Tile;

	Vec3 m_Speed = { 0,0,0 };
	Vec3 m_OnTileOffset = {.5f,.5f,0};

public:
	~GameObject();
	GameObject(WorldTile tile, Vec3 pos, int size);
	GameObject(WorldTile tile, Vec3 pos, int size, Direction direction);

	bool operator==(const GameObject& other);

	inline Vec3 GetPos() const { return m_WorldPos; }
	inline int GetSize() const { return m_Size; }
	inline Direction GetDirection() const { return m_Direction; }
	inline void SetDirection(Direction direction) { m_Direction = direction; }
	inline WorldTile GetTile() const { return m_Tile; }
	inline Vec3 GetSpeed() const { return m_Speed; }
	inline void SetSpeed(Vec3 speed) { m_Speed = speed; }
	inline Vec3 GetOnTileOffset() const { return m_OnTileOffset; }
	bool ChangeOnTileOffset(Vec3 change);

	bool Update();
	void AddQuad(int WorldTileSize);
};