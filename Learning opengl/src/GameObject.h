#pragma once
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

	WorldTile m_Tile;
	Vec3 m_Pos;
	int m_Size;
	Direction m_Direction;

public:
	inline WorldTile* GetTile() { return &m_Tile; }
	inline WorldTile GetTileCopy() const { return m_Tile; }
	inline void SetTile(WorldTile* tile) { m_Tile = *tile; }
	inline void SetPos(Vec3 pos) { m_Pos = pos; }
	inline Vec3 GetPos() const { return m_Pos; }
	inline int GetSize() const { return m_Size; }
	inline void SetSize(int size) { m_Size = size; }
	inline Direction GetDirection() const { return m_Direction; }
	inline void SetDirection(Direction direction) { m_Direction = direction; }

	void update() {}
};