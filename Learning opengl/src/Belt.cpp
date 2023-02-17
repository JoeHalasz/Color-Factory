#include "Belt.h"
#include <memory>


// this function will not deal with connecting the other belts to this belt 
Belt::Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType)
	:m_Tile(tile), m_Pos(pos), m_Size(size), m_Direction(direction), m_BeltType(beltType)
{

	if (beltType == BeltTypeYellow) m_ArrowTile = TileType(TileTypeYellowArrow);
	if (beltType == BeltTypeOrange) m_ArrowTile = TileType(TileTypeOrangeArrow);
	if (beltType == BeltTypeRed) m_ArrowTile = TileType(TileTypeRedArrow);

	BELT_SPEED = (1.0f / FramesTillMovedFullTile) * (m_BeltType + 1);
	m_MaxItemMoves = (float)FramesTillMovedFullTile / (m_BeltType + 1);
}

void Belt::SetUpNextAndLastBelt(std::unordered_map<int, std::unordered_map<int, std::vector<std::shared_ptr<Belt>>>>& AllOtherBelts)
{
	m_NextBelt = GetNextOrLastBelt(false, AllOtherBelts);
	m_LastBelt = GetNextOrLastBelt(true, AllOtherBelts);
	int here = 0;
	if (m_NextBelt != NULL) {
		while (m_NextBelt->m_LastBelt == NULL || m_NextBelt->m_LastBelt != this->shared_from_this()) { // not sure why this needs to happen more than once, but it does. Sometimes will not set it correctly
			m_NextBelt->m_LastBelt = this->shared_from_this();
			if (here++ >= 10)
				std::cout << "Im stuck help2" << std::endl; // keep this here
		}
	}
	if (m_LastBelt != NULL) {
		while (m_LastBelt->m_NextBelt == NULL || m_LastBelt->m_NextBelt != this->shared_from_this()) {
			m_LastBelt->m_NextBelt = this->shared_from_this();
			if (here++ >= 10)
				std::cout << "Im stuck help2" << std::endl; // keep this here
		}
		
	}
}


Belt::~Belt()
{
	if (m_NextBelt != NULL)
		m_NextBelt->m_LastBelt = NULL;
	if (m_LastBelt != NULL)
		m_LastBelt->m_NextBelt = NULL;
}


void Belt::Update()
{
	if (UpdatedThisFrame)
		return;
	UpdatedThisFrame = true;
	if (m_NextBelt == NULL)
		m_LastItemMoved = true;

	for (int i = 0; i < m_ObjectsOnBelt.size(); i++)
	{
		if (m_ObjectNumMoves[i] == m_MaxItemMoves) // this can only happen with the first element
		{
			if (m_NextBelt != NULL)
			{
				bool nextItemAllowed = m_NextBelt->AllowNewItem();
				if (m_NextBelt->AllowNewItem()) // move it from this belt to the next belt
				{
					m_NextBelt->m_ObjectsOnBelt.push_back(m_ObjectsOnBelt[i]);
					if (m_NextBelt->GetTile()->GetType() == TileTypeTurnBelt || m_NextBelt->GetTile()->GetType() == TileTypeTurnBeltBackwards)
						m_NextBelt->m_ObjectNumMoves.push_back(m_MaxItemMoves / 2);
					else
						m_NextBelt->m_ObjectNumMoves.push_back(0);
					MovePaintBlob(i);
					m_ObjectsOnBelt.erase(m_ObjectsOnBelt.begin() + i);
					m_ObjectNumMoves.erase(m_ObjectNumMoves.begin() + i);
					i--;
				}
				else {
					m_LastItemMoved = false;
				}
			}
			else {
				m_LastItemMoved = false;
			}
		}
		else
		{
			if (i == 0) MovePaintBlob(i);
			else if (m_LastItemMoved) MovePaintBlob(i);
			else
			{
				// if move will make it too close to next then dont move
				if (m_ObjectNumMoves[i - 1] - m_ObjectNumMoves[i] < m_MinSpaceBetween) // then it will be too close
					m_LastItemMoved = false;
				else 
					MovePaintBlob(i);
			}
		}
	}

	// update next in chain
	if (m_LastBelt != NULL)
		m_LastBelt->Update();
}

bool Belt::AllowNewItem(bool StartAtHalf)
{
	if (m_ObjectsOnBelt.size() == 0)
		return true;
	int spot = m_MinSpaceBetween;
	if (StartAtHalf || GetTile()->GetType() == TileTypeTurnBelt || GetTile()->GetType() == TileTypeTurnBeltBackwards)
		spot += m_MaxItemMoves / 2;
	if (m_ObjectNumMoves[m_ObjectNumMoves.size() - 1] >= spot)
		return true;
	return false;
}

std::shared_ptr<Belt> Belt::GetNextOrLastBelt(bool isLastBelt, std::unordered_map<int, std::unordered_map<int, std::vector<std::shared_ptr<Belt>>>>& AllOtherBelts)
{
	float xPos = GetPos().x;
	float yPos = GetPos().y;
	std::unique_ptr<Belt> fake;

	if (GetTile()->GetType() == TileTypeStraightBelt || !isLastBelt) // do this if we are a straight belt or looking for the next belt
	{
		int signFlip = 1;
		if (isLastBelt)
			signFlip = -1;

		if (GetDirection() == DirectionUp)
			yPos += 1 * signFlip;
		else if (GetDirection() == DirectionDown)
			yPos += -1 * signFlip;
		else if (GetDirection() == DirectionLeft)
			xPos += -1 * signFlip;
		else if (GetDirection() == DirectionRight)
			xPos += 1 * signFlip;

		
		// if there isnt a belt there 
		if (AllOtherBelts[xPos][yPos].size() == 0)
			return fake;

		std::shared_ptr<Belt> possibleNextBelt = AllOtherBelts[xPos][yPos][0];

		if (possibleNextBelt->GetTile()->GetType() != TileTypeTurnBelt && possibleNextBelt->GetTile()->GetType() != TileTypeTurnBeltBackwards)
			if (possibleNextBelt->m_Direction != GetDirection())
				return fake;

		return AllOtherBelts[xPos][yPos][0];
	}
	else // if we are a turn belt and we are looking for the last belt
	{
		Direction wantedDirection;
		if (GetTile()->GetType() == TileTypeTurnBelt)
		{
			if (GetDirection() == DirectionUp) {
				xPos += -1;
				wantedDirection = DirectionRight;
			}
			else if (GetDirection() == DirectionDown) {
				xPos += 1;
				wantedDirection = DirectionLeft;
			}
			else if (GetDirection() == DirectionLeft) {
				yPos += 1;
				wantedDirection = DirectionDown;
			}
			else if (GetDirection() == DirectionRight) {
				yPos += -1;
				wantedDirection = DirectionUp;
			}
		}
		else // turn belt backwards
		{
			if (GetDirection() == DirectionUp) {
				xPos += 1;
				wantedDirection = DirectionLeft;
			}
			else if (GetDirection() == DirectionDown) {
				xPos += -1;
				wantedDirection = DirectionRight;
			}
			else if (GetDirection() == DirectionLeft) {
				yPos += -1;
				wantedDirection = DirectionUp;
			}
			else if (GetDirection() == DirectionRight) {
				yPos += 1;
				wantedDirection = DirectionDown;
			}
		}

		// if there isnt a belt there 
		if (AllOtherBelts[xPos][yPos].size() == 0)
			return fake;

		std::shared_ptr<Belt> possibleNextBelt = AllOtherBelts[xPos][yPos][0];

		if (possibleNextBelt->GetDirection() != wantedDirection)
			return fake;

		return AllOtherBelts[xPos][yPos][0];
	}
}


void Belt::MovePaintBlob(int pos)
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

	float toCheck = .5;
	if (m_Direction == DirectionUp || m_Direction == DirectionDown)
	{
		offset = round((m_ObjectsOnBelt[pos].GetPos().x - int(GetPos().x)) * 100) / 100;
		if (offset < toCheck)
			moveTo.x = BELT_SPEED * (m_BeltType + 1);
		else if (offset > toCheck)
			moveTo.x = -1 * BELT_SPEED * (m_BeltType + 1);
	}
	else
	{
		offset = round((m_ObjectsOnBelt[pos].GetPos().y - int(GetPos().y)) * 100) / 100;
		if (offset < toCheck)
			moveTo.y = BELT_SPEED * (m_BeltType + 1);
		else if (offset > toCheck)
			moveTo.y = -1 * BELT_SPEED * (m_BeltType + 1);
	}

	m_ObjectsOnBelt[pos].SetPos(m_ObjectsOnBelt[pos].GetPos() + moveTo);

}