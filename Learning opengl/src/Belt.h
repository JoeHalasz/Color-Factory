#pragma once
#include <vector>
#include "GameObject.h"
#include <cmath>
#include <cstdlib>

enum BeltType
{
	BeltTypeYellow = 0, // speed * 1
	BeltTypeOrange = 1,	// speed * 2
	BeltTypeRed = 3,	// speed * 4
};

class Belt
{
private:
	Belt* m_NextBelt = nullptr;
	Belt* m_LastBelt = nullptr;
	std::vector<GameObject> m_ObjectsOnBelt;
	std::vector<int> m_ObjectNumMoves;
	
	WorldTile m_Tile;
	Vec3 m_Pos;
	int m_Size;
	Direction m_Direction;
	BeltType m_BeltType;
	TileType m_ArrowTile;
	Vec3 m_ArrowOffset = { 0 };


	bool m_LastItemMoved = false;
	int FramesTillMovedFullTile = 144; // the number of frames the slowest belt should take to move an object across the entire belt
	float BELT_SPEED = 1.0f / FramesTillMovedFullTile; // updated in constructor using belt type
	int m_MaxItemMoves; // should be calculated when belt is created based on speed
	float m_MinSpaceBetween = FramesTillMovedFullTile/5; // min allowed space between items on the belt
	bool allowNewItem = true; // if this belt has space for a new item 



public:
	Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType,
		std::unordered_map<int, std::unordered_map<int, std::vector<Belt>>>& AllOtherBelts);
	~Belt();

	void Update();
	void MoveGameObject(int pos);
	Belt* GetNextOrLastBelt(Vec3 pos, Direction direction, bool isLastBelt, std::unordered_map<int, std::unordered_map<int, std::vector<Belt>>>& AllOtherBelts);

	bool operator==(Belt other) {
		return (GetPos() == other.GetPos() && GetDirection() == other.GetDirection()
			&& GetTile()->GetType() == other.GetTile()->GetType() &&
			GetBeltType() == other.GetBeltType());
	}

	inline WorldTile* GetTile() { return &m_Tile; }
	inline Vec3 GetPos() const { return m_Pos; }
	inline int GetSize() const { return m_Size; }
	inline Direction GetDirection() const { return m_Direction; }
	inline Vec3 GetArrowPos() const {return Vec3{GetPos().x + m_ArrowOffset.x,GetPos().y + m_ArrowOffset.y,GetPos().z + m_ArrowOffset.z};}
	inline TileType GetArrowTile() const { return m_ArrowTile; }
	inline Belt* GetNextBelt() const { return m_NextBelt; }
	inline Belt* GetLastBelt() const { return m_LastBelt; }
	inline bool GetAllowNewItem() const { return allowNewItem; }
	inline BeltType GetBeltType() const { return m_BeltType; }

	inline void SetDirection(Direction direction) { m_Direction = direction; }
	inline void AddObject(GameObject& object) {
		m_ObjectsOnBelt.emplace_back(object);
		m_ObjectNumMoves.push_back(0);
	}
	inline GameObject& GetObjectAt(int pos) { return m_ObjectsOnBelt[pos]; }
	inline std::vector<GameObject>& GetAllObjects() { return m_ObjectsOnBelt; }
	inline unsigned int GetNumObjects() { return m_ObjectsOnBelt.size(); }

};