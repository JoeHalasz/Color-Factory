#include "Truck.h"
#include <cmath>

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
	int numNodes = m_CurrentNode->GetConnectedNodes().size();
	// j is a random number between 0 and the number of connected nodes
	int j = rand() % numNodes; // pick a random direction to check so that all trucks dont follow the same path
	for (int i = 0; i < numNodes; i++)
	{
		if (j == numNodes)
			j = 0;
		// if the new nodes pos is closer to the target pos then move to it 
		if (Distance(m_CurrentNode->GetConnectedNodes()[j]->GetPos(), m_TargetStop->GetNode()->GetPos()) < Distance(m_CurrentNode->GetPos(), m_TargetStop->GetNode()->GetPos()))
		{
			m_CurrentNode = m_CurrentNode->GetConnectedNodes()[j];
			break;
		}
	}

	// return true if we are at the target stop
	return m_CurrentNode->GetPos() == m_TargetStop->GetNode()->GetPos();
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
	// change direction and position (one off from center of road) based on the direction the truck is moving
	// also move in Y direction first so that the truck doesnt move into a wall when going to a stop

	// TODO  SET THE DIRECTION THAT THE TRUCK SHOULD MOVE WHEN FIGURING OUT NEXT NODE AND JUST MOVE IN THAT DIRECTIOn
	
	
	
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