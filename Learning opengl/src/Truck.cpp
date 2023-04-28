#include "Truck.h"
#include <cmath>
#include <queue>

Truck::Truck(std::shared_ptr<TruckNode> spawnNode)
	:m_CurrentNode(spawnNode, DirectionUp)
{
	m_Pos = m_CurrentNode.first->GetPos();
}

Truck::Truck(PaintBlob paintBlob, std::shared_ptr<TruckNode> spawnNode)
	: m_PaintBlobType(paintBlob), m_CurrentNode(spawnNode, DirectionUp)
{
	m_Pos = m_CurrentNode.first->GetPos();
}

// will return false if the truck already has a paint blob type, and there are more than 0 paint blobs in the truck
bool Truck::SetPaintBlobType(PaintBlob paintBlob)
{
	if (m_NumPaintBlobs != 0)
		return false;
	m_PaintBlobType = paintBlob;
	return true;
}


// will return true if the paint blob was added
bool Truck::AddPaintBlob(PaintBlob paintBlob)
{
	if (paintBlob == m_PaintBlobType)
	{
		m_NumPaintBlobs++;
		return true;
	}
	return false;
}

// will return nullptr if there are no paint blobs
std::shared_ptr<PaintBlob> Truck::RemovePaintBlob()
{
	if (m_NumPaintBlobs > 0)
	{
		m_NumPaintBlobs--;
		return std::make_shared<PaintBlob>(m_PaintBlobType);
	}
	return nullptr;
}

int Distance(Vec3 a, Vec3 b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

int Distance(int a, int b)
{
	return abs(a - b);
}

float Distance(float a, float b)
{
	return abs(a - b);
}

// will return true if the truck is at the target stop
bool Truck::PickNextNode()
{	
	// get the next node in the path. If we are currently at the target stop, then return true
	if (m_CurrentNode.first == m_TargetStop->GetNode())
	{
		return true;
	}
	// set the current node to the next node in the path
	m_CurrentNode = m_Path[0];
	m_Path.erase(m_Path.begin());
	return false;
}


// will return true if there is a truck in the way, and its on the same side of the street (moving in the same direction)
bool Truck::CheckTruckInTheWay(Direction movementDirection)
{
	// if there is a truck on the next square and that truck is moving in the same direction as this truck, then return true
	if (movementDirection == DirectionUp)
	{
		if (m_AllTrucks[m_Pos.x][m_Pos.y - 1] != nullptr && m_AllTrucks[m_Pos.x][m_Pos.y - 1]->GetDirection() == GetDirection())
			return true;
	}
	else if (movementDirection == DirectionDown)
	{
		if (m_AllTrucks[m_Pos.x][m_Pos.y + 1] != nullptr && m_AllTrucks[m_Pos.x][m_Pos.y + 1]->GetDirection() == GetDirection())
			return true;
	}
	else if (movementDirection == DirectionLeft)
	{
		if (m_AllTrucks[m_Pos.x - 1][m_Pos.y] != nullptr && m_AllTrucks[m_Pos.x - 1][m_Pos.y]->GetDirection() == GetDirection())
			return true;
	}
	else if (movementDirection == DirectionRight)
	{
		if (m_AllTrucks[m_Pos.x + 1][m_Pos.y] != nullptr && m_AllTrucks[m_Pos.x + 1][m_Pos.y]->GetDirection() == GetDirection())
			return true;
	}
}


// will move the truck to the correct side of the street over time
void Truck::FixRoadOffset(Direction movementDirection)
{
	float targetPos;
	if (movementDirection == DirectionUp || movementDirection == DirectionDown)
	{
		targetPos = m_CurrentNode.first->GetPos().x;
	}
	else
	{
		targetPos = m_CurrentNode.first->GetPos().y;
	}
	if (m_CurrentNode.first->GetIsStop())
	{
		if (movementDirection == DirectionUp)
		{
			targetPos += 2; // move it over by 2 so its centered
		}
		if (movementDirection == DirectionDown)
		{
			targetPos -= 3; // same as last but other side
		}
	}
	// if we are moving up then move the truck to the right until one off the node
	if (movementDirection == DirectionUp)
	{
		if (m_Pos.x < targetPos+1 && Distance(m_Pos.x, targetPos + 1) > .1f)
			m_Pos.x += .1f;
		else if (m_Pos.x > targetPos+1 && Distance(m_Pos.x, targetPos + 1) > .1f)
			m_Pos.x -= .1f;
	}
	else if (movementDirection == DirectionDown)
	{
		if (m_Pos.x < targetPos - 1 && Distance(m_Pos.x, targetPos - 1) > .1f)
			m_Pos.x += .1f;
		else if (m_Pos.x > targetPos - 1 && Distance(m_Pos.x, targetPos - 1) > .1f)
			m_Pos.x -= .1f;
	}
	else if (movementDirection == DirectionLeft)
	{
		if (m_Pos.y < targetPos + 1 && Distance(m_Pos.y, targetPos + 1) > .1f)
			m_Pos.y += .1f;
		else if (m_Pos.y > targetPos + 1 && Distance(m_Pos.y, targetPos + 1) > .1f)
			m_Pos.y -= .1f;
	}
	else if (movementDirection == DirectionDown)
	{
		if (m_Pos.y < targetPos - 1 && Distance(m_Pos.y, targetPos - 1) > .1f)
			m_Pos.y += .1f;
		else if (m_Pos.y > targetPos - 1 && Distance(m_Pos.y, targetPos - 1) > .1f)
			m_Pos.y -= .1f;
	}
}

// this function will return true if the target node is not a stop, or if it is a stop and it is close enough to the stop
bool Truck::CheckMoveToStop()
{
	// if the target node is not a stop, then return true
	if (!m_CurrentNode.first->GetIsStop())
		return true;
	// if the target node is a stop, then check if we are close enough to the stop
	std::cout << "Here" << std::endl; // only here once, then it moves to the next node
	if (m_CurrentNode.second == DirectionUp || m_CurrentNode.second == DirectionDown)
	{
		if (Distance(m_Pos.x, m_CurrentNode.first->GetPos().x) <= .1f) 
			return true;
	}
	else if (m_CurrentNode.second == DirectionLeft || m_CurrentNode.second == DirectionRight)
	{
		if (Distance(m_Pos.y, m_CurrentNode.first->GetPos().y) <= .1f)
			return true;
	}
	// move towards the stop
	if (m_CurrentNode.second == DirectionUp) // can only be up or down cause its a stop
		m_Pos.x -= .1f;
	else if (m_CurrentNode.second == DirectionDown)
		m_Pos.x += .1f;
	return false;
}

// will return true if the truck is at the target node
bool Truck::MoveTowardsNextNode()
{
	// if we are within .1 of the current node, go to next node
	Direction directionToMove = m_CurrentNode.second;
	if ((directionToMove == DirectionUp || directionToMove == DirectionDown) && Distance(m_Pos.y, m_CurrentNode.first->GetPos().y) <= .1f)
		return CheckMoveToStop();

	if ((directionToMove == DirectionLeft || directionToMove == DirectionRight) && Distance(m_Pos.x, m_CurrentNode.first->GetPos().x) <= .1f)
		return CheckMoveToStop();
	
	// dont move if there is another truck in the way
	if (CheckTruckInTheWay(directionToMove))
		return false;
	
	// we are not close enough yet, so move towards the node
	if (directionToMove == DirectionUp)
		m_Pos.y += .1f;
	else if (directionToMove == DirectionDown)
		m_Pos.y -= .1f;
	else if (directionToMove == DirectionLeft)
		m_Pos.x -= .1f;
	else if (directionToMove == DirectionRight)
		m_Pos.x += .1f;
	
	// correct for side of the street
	FixRoadOffset(directionToMove);
	
	return false; // if we made it to here then we moved, so we are not at the node yet
}

void Truck::Update()
{
	if (!m_AtStop)
	{
		if (MoveTowardsNextNode()) // then we are at the next node
		{
			if (PickNextNode()) // then we are at the target node
			{
				m_AtStop = true;
				// if we have an inventory, then unload it
				m_Unloading = true;
			}
		}
	}
	else
	{
		// if we are unloading and have an inventory, then try to unload
		if (m_Unloading)
		{
			if (m_NumPaintBlobs > 0) {
				// if the stop has space for the paint blob, then unload it
				if (m_TargetStop->GetPaintBlobType() == m_PaintBlobType)
				{
					if (m_TargetStop->AddPaintBlob())
					{
						RemovePaintBlob();
					}
				}
				// else truck is waiting for stop's inv to clear
			}
			else
			{
				m_Unloading = false;
				m_WaitingForTask = true;
			}
		}
		if (m_Loading)
		{
			if (m_NumPaintBlobs < m_MaxPaintBlobs)
			{
				
				if (m_TargetStop->RemovePaintBlob()) // try to add one, THIS WILL ADD ONE IF IT CAN
				{
					if (!AddPaintBlob(m_TargetStop->GetPaintBlobType()))
					{
						m_TargetStop->AddPaintBlob(); // if we couldnt get one from the station then readd the one we just removed
					}
					else
					{
						// stop now has a different kind of paint
						m_Loading = false;
						m_WaitingForTask = true;
					}
				}
				else
				{
					m_Loading = false;
					m_WaitingForTask = true;
				}
			}
			else
			{
				m_Loading = false;
				m_WaitingForTask = true; // task handler will figure out where truck full of this paint should go
			}
		}
	}
}

struct pair_hash {
	std::size_t operator () (const std::pair<std::shared_ptr<TruckNode>, Direction>& p) const {
		// use the hash of the node's position and the direction
		return std::hash<int>()(p.first->GetPos().x) ^ (std::hash<int>()(p.first->GetPos().y) * std::hash<int>()(p.second));
	}
};


std::vector<std::pair<std::shared_ptr<TruckNode>, Direction>> findPath(std::pair<std::shared_ptr<TruckNode>, Direction> startNode, std::shared_ptr<TruckNode> targetNode)
{
	// use BFS search to find the shortest path to the target node
	std::vector<std::pair<std::shared_ptr<TruckNode>, Direction>> path;
	std::queue<std::pair<std::shared_ptr<TruckNode>, Direction>> q;
	std::unordered_map<std::pair<std::shared_ptr<TruckNode>, Direction>, std::pair<std::shared_ptr<TruckNode>, Direction>, pair_hash> parentMap;
	q.push(startNode);
	parentMap[startNode] = std::pair <std::shared_ptr<TruckNode>, Direction>(nullptr, DirectionUp);
	while (!q.empty())
	{
		std::pair<std::shared_ptr<TruckNode>, Direction> currentNode = q.front();
		q.pop();
		if (currentNode.first == targetNode)
		{
			// we found the target node, now we need to backtrack to find the path
			std::pair<std::shared_ptr<TruckNode>, Direction> currentPathNode = currentNode;
			while (currentPathNode != std::pair <std::shared_ptr<TruckNode>, Direction>(nullptr, DirectionUp))
			{
				path.push_back(currentPathNode);
				currentPathNode = parentMap[currentPathNode];
			}
			std::reverse(path.begin(), path.end());
			return path;
		}
		for (auto& node : currentNode.first->GetConnectedNodes())
		{
			if (parentMap.find(node) == parentMap.end())
			{
				q.push(node);
				parentMap[node] = currentNode;
			}
		}
	}
	// if we get here then we couldnt find a path
	return path;
}

void Truck::SetTargetStop(std::shared_ptr<TruckStop> targetStop) {
	m_TargetStop = targetStop;
	// find path to that stop
	m_Path = findPath(m_CurrentNode, m_TargetStop->GetNode());
	PickNextNode();
}