#pragma once
#include <vector>
#include <cmath>
#include <cstdlib>
#include "GameObject.h"
#include "PaintBlob.h"

enum BeltType
{
	BeltTypeYellow = 0, 
	BeltTypeOrange = 1,	
	BeltTypeRed = 2,	
};

class Belt : public GameObject, public std::enable_shared_from_this <Belt>
{
private:

	
	

	BeltType m_BeltType;
	TileType m_ArrowTile;
	Vec3 m_ArrowOffset = { 0 };

	bool m_LastItemMoved = false;
	int FramesTillMovedFullTile = 120;//MUST BE EVEN: the number of frames the slowest belt should take to move an object across the entire belt
	float BELT_SPEED; // updated in constructor using belt type
	int m_MaxItemMoves; // should be calculated when belt is created based on speed


public:
	bool UpdatedThisFrame = false;

	Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType);
	~Belt();

	void Update();
	void MovePaintBlob(int pos);
	std::shared_ptr<GameObject> GetNextOrLastObject(bool isLastBelt, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Belt>>>& AllOtherBelts,
															 std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>>& worldGameObjects);
	void SetUpNextAndLastObject(std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Belt>>>& AllOtherBelts,
							  std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>>& worldGameObjects);
	


	bool operator==(Belt& other) {
		return (GetPos() == other.GetPos() && GetDirection() == other.GetDirection()
			&& GetTile()->GetType() == other.GetTile()->GetType() &&
			GetBeltType() == other.GetBeltType());
	}


	inline Vec3 GetArrowPos() const {return Vec3{GetPos().x + m_ArrowOffset.x,GetPos().y + m_ArrowOffset.y,GetPos().z + m_ArrowOffset.z};}
	inline TileType GetArrowTile() const { return m_ArrowTile; }
	inline BeltType GetBeltType() const { return m_BeltType; }

	inline void AddObject(std::shared_ptr<PaintBlob> object, bool StartAtHalf = false) {
		if (object == NULL)
			return;
		getObjectInInv().push_back(object);
		if (StartAtHalf)
			getObjectNumMoves().push_back(m_MaxItemMoves / 2);
		else
			getObjectNumMoves().push_back(0);
	}
	inline std::shared_ptr<GameObject> GetObjectAt(int pos) { return getObjectInInv()[pos]; }
	inline std::vector<std::shared_ptr<GameObject>> GetAllObjects() { return getObjectInInv(); }
	inline unsigned int GetNumObjects() { return getObjectInInv().size(); }
	inline float GetBeltTypeSpeed() {
		switch (m_BeltType) {
			case(0): return 1.0f; break;
			case(1): return 1.2f; break;
			case(2): return 1.4f; break;
		}
	}
	bool AllowNewItem(bool StartAtHalf = false);
};