#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>
#include "glm/glm.hpp"
#include "WorldTile.h"

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

public:
	~GameObject();
	GameObject(WorldTile tile, Vec3 pos, int size);
	GameObject(WorldTile tile, Vec3 pos, int size, Direction direction);

	bool operator==(const GameObject& other);

	inline Vec3 GetPos() const { return m_WorldPos; }
	inline void SetPos(Vec3 pos) { m_WorldPos = pos; }
	inline int GetSize() const { return m_Size; }
	inline Direction GetDirection() const { return m_Direction; }
	inline void SetDirection(Direction direction) { m_Direction = direction; }
	inline WorldTile* GetTile() { return &m_Tile; }
	inline WorldTile GetTileCopy() const { return m_Tile; }
	inline Vec3 GetSpeed() const { return m_Speed; }
	inline void SetSpeed(Vec3 speed) { m_Speed = speed; }

	void MoveBy(Vec3 amount, std::unordered_map<int, std::unordered_map<int, std::vector<GameObject>>>& gameObjects);

	bool Update();
	void AddQuad(int WorldTileSize);
};