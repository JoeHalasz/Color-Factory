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

class Belt : public GameObject
{
private:
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
	
	Vec3 GetArrowPos() const;
};