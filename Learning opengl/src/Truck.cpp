#include "Truck.h"
#include <cmath>
#include <queue>

Truck::Truck(std::shared_ptr<TruckNode> spawnNode)
	:m_CurrentNode(spawnNode)
{
	m_Pos = m_CurrentNode->GetPos();
}

Truck::Truck(PaintBlob paintBlob, std::shared_ptr<TruckNode> spawnNode)
	: m_PaintBlobType(paintBlob), m_CurrentNode(spawnNode)
{
	m_Pos = m_CurrentNode->GetPos();
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

// will return true if the truck is at the target stop
bool Truck::PickNextNode()
{	
	// get the next node in the path. If we are currently at the target stop, then return true
	if (m_CurrentNode == m_TargetStop->GetNode())
	{
		return true;
	}
	// set the current node to the next node in the path
	m_CurrentNode = m_Path[0];
	m_Path.erase(m_Path.begin());
	return false;
}

// will return true if the truck is at the target node
bool Truck::MoveTowardsNextNode()
{
	float amountToAdd = 1;
	float amountToSubtract = -1;
	if (m_CurrentNode->GetIsStop())
	{
		amountToAdd = 4;
		amountToSubtract = 1;
	}

	// if we are not within .1 of the cuurent node, then move towards it
	if (Distance(m_Pos, m_CurrentNode->GetPos()) > .1f)
	{
		// move in the y direction
		if (m_Pos.y < m_CurrentNode->GetPos().y && Distance(m_Pos.y, m_CurrentNode->GetPos().y) > .1f)
		{
			m_Pos.y += .1f;
		}
		else if (m_Pos.y > m_CurrentNode->GetPos().y && Distance(m_Pos.y, m_CurrentNode->GetPos().y) > .1f)
		{
			m_Pos.y += -.1f;
		}
		// move in the x direction
		else if (m_Pos.x < m_CurrentNode->GetPos().x && Distance(m_Pos.x, m_CurrentNode->GetPos().x) > .1f)
		{
			m_Pos.x += .1f;
		}
		else if (m_Pos.x > m_CurrentNode->GetPos().x && Distance(m_Pos.x, m_CurrentNode->GetPos().x) > .1f)
		{
			m_Pos.x += -.1f;
		}
		
		return false;
	}
	// if we are within .1 of the current node, then move to the next node
	return true;





	// change direction and position (one off from center of road) based on the direction the truck is moving
	// also move in Y direction first so that the truck doesnt move into a wall when going to a stop


	// TODO  SET THE DIRECTION THAT THE TRUCK SHOULD MOVE WHEN FIGURING OUT NEXT NODE AND JUST MOVE IN THAT DIRECTIOn
	// also do truck collision, need to save all trucks in constructor
	
	
	return false;
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

std::vector<std::shared_ptr<TruckNode>> findPath(std::shared_ptr<TruckNode> startNode, std::shared_ptr<TruckNode> targetNode)
{
	// use BFS search to find the shortest path to the target node
	std::vector<std::shared_ptr<TruckNode>> path;
	std::queue<std::shared_ptr<TruckNode>> q;
	std::unordered_map<std::shared_ptr<TruckNode>, std::shared_ptr<TruckNode>> parentMap;
	q.push(startNode);
	parentMap[startNode] = nullptr;
	while (!q.empty())
	{
		std::shared_ptr<TruckNode> currentNode = q.front();
		q.pop();
		if (currentNode == targetNode)
		{
			// we found the target node, now we need to backtrack to find the path
			std::shared_ptr<TruckNode> currentPathNode = currentNode;
			while (currentPathNode != nullptr)
			{
				path.push_back(currentPathNode);
				currentPathNode = parentMap[currentPathNode];
			}
			std::reverse(path.begin(), path.end());
			return path;
		}
		for (auto& node : currentNode->GetConnectedNodes())
		{
			if (parentMap.find(node) == parentMap.end())
			{
				parentMap[node] = currentNode;
				q.push(node);
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