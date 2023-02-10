#pragma once
#include "GameObject.h"
#include <vector>
#include <unordered_map>



enum BeltType
{
	BeltTypeYellow	= 0,
	BeltTypeOrange	= 1,
	BeltTypeRed		= 2,
};

class Belt
{
private:
	Vec3 m_WorldPos = { 0,0,0 };
	int m_Size;
	Direction m_Direction = DirectionUp;
	WorldTile m_Tile;

	float BELT_SPEED = 1.0f/144.0f;
	BeltType m_BeltType = BeltTypeYellow;
	TileType m_ArrowTile;
	Vec3 m_ArrowOffset = { 0 };
	float m_ArrowSpeed = 0;
public:
	Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType);
	void Update(std::unordered_map<int, std::unordered_map<int, std::vector<GameObject>>>& gameObjects);
	inline TileType GetArrowTile() const { return m_ArrowTile; }
	inline Vec3 GetArrowOffset() const { return m_ArrowOffset; }

	inline Direction GetDirection() const { return m_Direction; }
	inline void SetDirection(Direction direction) { m_Direction = direction; }
	
	inline Vec3 GetPos() const { return m_WorldPos; }
	inline void SetPos(Vec3 pos) { m_WorldPos = pos; }
	inline int GetSize() const { return m_Size; }

	inline WorldTile* GetTile() { return &m_Tile; }
	inline WorldTile GetTileCopy() const { return m_Tile; }

	Vec3 GetArrowPos() const;
};