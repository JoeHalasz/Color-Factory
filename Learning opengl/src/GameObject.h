#pragma once
#include "WorldTile.h"
#include <vector>
#include "HelperFunctions.h"

class GameObject
{
private:

	WorldTile m_Tile;
	Vec3 m_Pos = { 0 };
	int m_Size = 0;
	Direction m_Direction = DirectionUp;

	std::shared_ptr<GameObject> m_NextObject;
	std::shared_ptr<GameObject> m_LastObject;

	bool m_IsBasePart = false; // this is true if this is not one of the connecting parts of a game object
	std::vector<std::shared_ptr<GameObject>> m_OtherParts; // contains the other parts of a game object (so rendered pieces of the object that are on different squares )
	std::shared_ptr<GameObject> m_ParentObject; // this will only exist if m_IsBasePart is true
	std::vector<std::shared_ptr<GameObject>> m_ObjectsInInv;
	std::vector<int> m_ObjectNumMoves;
	float m_MinSpaceBetween; // min allowed space between items on the belt
	int m_MaxItemMoves; // should be calculated when belt is created based on speed


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

	inline void setLastObject(std::shared_ptr<GameObject> obj) { m_LastObject = obj; }
	inline std::shared_ptr<GameObject> getLastObject() { return m_LastObject; }
	inline void setNextObject(std::shared_ptr<GameObject> obj) { m_NextObject = obj; }
	inline std::shared_ptr<GameObject> getNextObject() { return m_NextObject; }
	inline int getMaxItemMoves() { return m_MaxItemMoves; }
	inline void setMaxItemMoves(int maxItemMoves) { m_MaxItemMoves = maxItemMoves; }

	inline std::vector<std::shared_ptr<GameObject>>& getObjectsInInv() { return m_ObjectsInInv; }
	inline void AddObject(std::shared_ptr<GameObject> object, bool StartAtHalf = false) {
		if (object == NULL)
			return;
		getObjectsInInv().push_back(object);
		if (GetTile()->GetType() == TileTypeTurnBelt || GetTile()->GetType() == TileTypeTurnBeltBackwards || StartAtHalf)
			getObjectNumMoves().push_back(m_MaxItemMoves / 2);
		else
			getObjectNumMoves().push_back(0);
	}
	inline std::shared_ptr<GameObject> GetObjectAt(int pos) { return getObjectsInInv()[pos]; }
	
	inline std::vector<int>& getObjectNumMoves() { return m_ObjectNumMoves; }

	inline float getMinSpaceBetween() { return m_MinSpaceBetween; }
	inline void SetMinSpaceBetween(float minSpaceBetween) { m_MinSpaceBetween = minSpaceBetween; }

	inline bool OnSamePos(GameObject& other) {
		return GetPos().x == other.GetPos().x && GetPos().y == other.GetPos().y;
	}

	virtual void Update() {}
	virtual void Render() { std::cout << "Need to create a render for new object" << std::endl; }
	virtual bool AllowNewItem(bool StartAtHalf = false) { return false; }
};