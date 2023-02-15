#include "Belt.h"
#include <memory>


Belt::Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType, 
	std::unordered_map<int, std::unordered_map<int, std::vector<std::shared_ptr<Belt>>>>& AllOtherBelts)
	:m_Tile(tile), m_Pos(pos), m_Size(size), m_Direction(direction), m_BeltType(beltType)
{
	m_NextBelt = GetNextOrLastBelt(pos, direction, false, AllOtherBelts);
	m_LastBelt = GetNextOrLastBelt(pos, direction, true, AllOtherBelts);

	std::cout << "THESE SHOULD BE THE SAME" << this << "=" << &AllOtherBelts[GetPos().x][GetPos().y][0] << std::endl;

	if (m_NextBelt != NULL) {
		m_NextBelt->m_LastBelt = this->shared_from_this();
		std::cout << m_NextBelt->m_LastBelt << " This needs to be something" << std::endl;
	}
	if (m_LastBelt != NULL) {
		m_LastBelt->m_NextBelt = this->shared_from_this();
		std::cout << "Next belt is " << m_LastBelt->m_NextBelt << "also this is " << this << std::endl;
	}

	if (beltType == BeltTypeYellow) m_ArrowTile = TileType(TileTypeYellowArrow);
	if (beltType == BeltTypeOrange) m_ArrowTile = TileType(TileTypeOrangeArrow);
	if (beltType == BeltTypeRed) m_ArrowTile = TileType(TileTypeRedArrow);

	BELT_SPEED *= (m_BeltType + 1);
	m_MaxItemMoves = FramesTillMovedFullTile / (m_BeltType + 1);
}


Belt::~Belt()
{
	std::cout << " DESTROYING BELT " << std::endl;
	if (m_NextBelt != NULL)
		m_NextBelt->m_LastBelt = NULL;
	if (m_LastBelt != NULL)
		m_LastBelt->m_NextBelt = NULL;
}


void Belt::Update()
{
	if (m_NextBelt == NULL)
		m_LastItemMoved = true;

	for (int i = 0; i < m_ObjectsOnBelt.size(); i++)
	{
		std::cout << GetPos().x << ", " << GetPos().y << " has something on it! " << m_ObjectNumMoves[i] << std::endl;
		if (m_ObjectNumMoves[i] == m_MaxItemMoves) // this can only happen with the first element
		{
			if (m_NextBelt != NULL)
			{
				if (m_NextBelt->allowNewItem) // move it from this belt to the next belt
				{
					m_NextBelt->m_ObjectsOnBelt.push_back(m_ObjectsOnBelt[i]);
					m_NextBelt->m_ObjectNumMoves.push_back(0);
					MoveGameObject(i);
					m_ObjectsOnBelt.erase(m_ObjectsOnBelt.begin() + i);
					m_ObjectNumMoves.erase(m_ObjectNumMoves.begin() + i);
					i--;
				}
			}
			else {
				m_LastItemMoved = false;
				std::cout << "No next belt found" << std::endl;
			}
		}
		else
		{
			if (i == 0) MoveGameObject(i);
			else if (m_LastItemMoved) MoveGameObject(i);
			else
			{
				// if move will make it too close to next then dont move
				if (m_ObjectNumMoves[i - 1] - m_ObjectNumMoves[i] < m_MinSpaceBetween) // then it will be too close
					m_LastItemMoved = false;
				else 
					MoveGameObject(i);
			}
		}
	}
	if (m_ObjectsOnBelt.size() == 0 || m_ObjectNumMoves[m_ObjectNumMoves.size() - 1] >= FramesTillMovedFullTile - m_MinSpaceBetween)
		allowNewItem = true;
	else
		allowNewItem = false;
	// update next in chain
	if (m_LastBelt != NULL)
		m_LastBelt->Update();
}

std::shared_ptr<Belt> Belt::GetNextOrLastBelt(Vec3 pos, Direction direction, bool isLastBelt, std::unordered_map<int, std::unordered_map<int, std::vector<std::shared_ptr<Belt>>>>& AllOtherBelts)
{
	float xPos = pos.x;
	float yPos = pos.y;
	int signFlip = 1;

	if (isLastBelt)
		signFlip = -1;

	if (direction == DirectionUp)
		yPos += 1 * signFlip;
	else if (direction == DirectionDown)
		yPos += -1 * signFlip;
	else if (direction == DirectionLeft)
		xPos += -1 * signFlip;
	else if (direction == DirectionRight)
		xPos += 1 * signFlip;

	std::unique_ptr<Belt> fake;

	if (AllOtherBelts[xPos][yPos].size() == 0)
		return fake;

	std::shared_ptr<Belt> possibleNextBelt = AllOtherBelts[xPos][yPos][0];

	if (possibleNextBelt->m_Direction != direction) 
		return fake;

	return AllOtherBelts[xPos][yPos][0];
}


void Belt::MoveGameObject(int pos)
{
	m_LastItemMoved = true;
	m_ObjectNumMoves[pos]++;
	Vec3 moveTo = { 0,0,1 };
	// move by direction and speed
	float offset; float direction;
	if (m_Direction == DirectionUp) 
		moveTo.y = BELT_SPEED * (m_BeltType + 1);
	else if (m_Direction == DirectionDown) 
		moveTo.y = -1 * BELT_SPEED * (m_BeltType + 1);
	else if (m_Direction == DirectionRight) 
		moveTo.x = BELT_SPEED * (m_BeltType + 1);
	else if (m_Direction == DirectionLeft) 
		moveTo.x = -1 * BELT_SPEED * (m_BeltType + 1);	


	// move to middle of the belt on turns 
	if (m_Direction == DirectionUp || m_Direction == DirectionDown)
	{
		offset = m_ObjectsOnBelt[pos].GetPos().x - std::floor(m_ObjectsOnBelt[pos].GetPos().x);
		if (std::abs(offset) != .5f) {
			direction = .5f - offset;
			direction /= std::abs(direction);
			moveTo.x = BELT_SPEED * (m_BeltType + 1) * direction;
		}
	}
	else
	{
		offset = m_ObjectsOnBelt[pos].GetPos().y - std::floor(m_ObjectsOnBelt[pos].GetPos().y);
		if (std::abs(offset) != .5f) {
			direction = .5f - offset;
			direction /= std::abs(direction);
			moveTo.y = BELT_SPEED * (m_BeltType + 1) * direction;
		}
	}

	m_ObjectsOnBelt[pos].SetPos(m_ObjectsOnBelt[pos].GetPos() + moveTo);

}