#include <deque>
#include <PaintBlob.h>
#include <TruckNode.h>
#include <TruckStop.h>

class Truck
{
private:
	// a truck can only have one type of paint blob
	PaintBlob m_PaintBlobType = PaintBlob({ 0,0,0 }, 0, {0,0,0,0}, DirectionUp);
	int m_NumPaintBlobs = 0;
	int m_MaxPaintBlobs = 1000;
	std::pair<std::shared_ptr<TruckNode>, Direction> m_CurrentNode;
	std::vector<std::pair<std::shared_ptr<TruckNode>, Direction>> m_Path;
	std::shared_ptr<TruckStop> m_TargetStop;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Truck>>> m_AllTrucks;

	Vec3 m_Pos;
	Direction m_Direction = DirectionUp; // should be based off of which way the truck is moving

	bool m_AtStop = false;
	bool m_WaitingForTask = false; // this should start as true later TODO
	bool m_Unloading = false;
	bool m_Loading = false;

public:
	Truck(std::shared_ptr<TruckNode> spawnNode);
	Truck(PaintBlob paintBlob, std::shared_ptr<TruckNode> spawnNode);
	
	inline int GetNumPaintBlobs() const { return m_NumPaintBlobs; }
	inline void SetPos(Vec3 pos) { m_Pos = pos; }
	inline Vec3 GetPos() const { return m_Pos; }
	inline void SetDirection(Direction direction) { m_Direction = direction; }
	inline Direction GetDirection() const { return m_Direction; }
	
	bool CheckMoveToStop();
	bool CheckTruckInTheWay(Direction movementDirection);
	void FixRoadOffset(Direction movementDirection);
	void SetTargetStop(std::shared_ptr<TruckStop> targetStop);
	bool SetPaintBlobType(PaintBlob paintBlob);
	bool AddPaintBlob(PaintBlob paintBlob);
	std::shared_ptr<PaintBlob> RemovePaintBlob();
	bool PickNextNode();
	bool MoveTowardsNextNode();
	void Update();
};