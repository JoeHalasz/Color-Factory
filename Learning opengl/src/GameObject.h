#pragma once
#include "WorldTile.h"
#include <vector>

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

	bool m_IsBasePart; // this is true if this is not one of the connecting parts of a game object
	std::vector<std::shared_ptr<GameObject>> m_OtherParts; // contains the other parts of a game object (so rendered pieces of the object that are on different squares )
	std::shared_ptr<GameObject> m_ParentObject; // this will only exist if m_IsBasePart is true

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
	
	inline void SetIsBasePart(bool isBasePart) { m_IsBasePart = !isBasePart; }
	inline bool GetIsBasePart() { return m_IsBasePart; }
	inline void AddOtherPart(std::shared_ptr<GameObject> part) { m_OtherParts.push_back(part); }
	inline std::vector<std::shared_ptr<GameObject>> GetOtherParts() { return m_OtherParts; }
	inline void SetParentObject(std::shared_ptr<GameObject> parent) { m_ParentObject = parent; }
	inline std::shared_ptr<GameObject> GetParentObject() { return m_ParentObject; }

	virtual void update() {}
	virtual void Render() { std::cout << "Need to create a render for new object" << std::endl; }
};