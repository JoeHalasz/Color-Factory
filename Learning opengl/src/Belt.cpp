#include "Belt.h"
#include <memory>


// this function will not deal with connecting the other belts to this belt 
Belt::Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType)
	: m_BeltType(beltType)
{
	SetTile(&tile);
	SetPos(pos);
	SetSize(size);
	SetDirection(direction);

	BELT_SPEED = (1.0f / FramesTillMovedFullTile) * (GetBeltTypeSpeed());
	setMaxItemMoves((int)((float)FramesTillMovedFullTile / ((GetBeltTypeSpeed() * GetBeltTypeSpeed())))); // fix this equation. Might be the .5 thing
	
	if (beltType == BeltTypeYellow) { m_ArrowTile = TileType(TileTypeYellowArrow); SetMinSpaceBetween((float)getMaxItemMoves() / 5); }
	if (beltType == BeltTypeOrange) { m_ArrowTile = TileType(TileTypeOrangeArrow); SetMinSpaceBetween((float)(getMaxItemMoves() / 5) + 1); }
	if (beltType == BeltTypeRed)	{ m_ArrowTile = TileType(TileTypeRedArrow	); SetMinSpaceBetween((float)(getMaxItemMoves() / 5) + 1); }
}

void Belt::SetUpNextAndLastObject(std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Belt>>>& AllOtherBelts, 
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>>& worldGameObjects)
{
	setNextObject(GetNextOrLastObject(false, AllOtherBelts, worldGameObjects));
	setLastObject(GetNextOrLastObject(true, AllOtherBelts, worldGameObjects));
	int tries = 0;
	if (getNextObject() != NULL) {
		while (getNextObject()->getLastObject() == NULL || getNextObject()->getLastObject() != this->shared_from_this()) { // not sure why this needs to happen more than once, but it does. Sometimes will not set it correctly
			getNextObject()->setLastObject(this->shared_from_this());
			if (tries++ >= 10)
				std::cout << "Im stuck help" << std::endl; // keep this here
		}
	}
	if (getLastObject() != NULL) {
		while (getLastObject()->getNextObject() == NULL || getLastObject()->getNextObject() != this->shared_from_this()) {
			getLastObject()->setNextObject(this->shared_from_this());
			if (tries++ >= 10)
				std::cout << "Im stuck help2" << std::endl; // keep this here
		}
	}
}


Belt::~Belt()
{
	if (getNextObject() != NULL)
		getNextObject()->setLastObject(NULL);
	if (getLastObject() != NULL)
		getLastObject()->setNextObject(NULL);
}


void Belt::Update()
{
	if (UpdatedThisFrame)
		return;
	UpdatedThisFrame = true;
	if (getNextObject() == NULL)
		m_LastItemMoved = true;

	if (getObjectsInInv().size() == 0)
		m_LastItemMoved = true;
	for (unsigned int i = 0; i < getObjectsInInv().size(); i++)
	{
		if (getObjectNumMoves()[i] == getMaxItemMoves()) // this can only happen with the first element
		{
			if (getNextObject() != NULL)
			{
				bool nextItemAllowed = getNextObject()->AllowNewItem();
				if (getNextObject()->AllowNewItem()) // move it from this belt to the next belt
				{
					int numObjectsInNext = getNextObject()->getObjectsInInv().size();
					getNextObject()->AddObject(getObjectsInInv()[i]);
					MovePaintBlob(i);
					if (numObjectsInNext != getNextObject()->getObjectsInInv().size()) {
						getObjectsInInv().erase(getObjectsInInv().begin() + i);
						getObjectNumMoves().erase(getObjectNumMoves().begin() + i);
						i--;
					}
				}
				else {
					m_LastItemMoved = false;
				}
			}
			else { // TODO (maybe) put it on the floor on the next tile 
				m_LastItemMoved = false;
			}
		}
		else
		{
			if (i == 0)
			{
				// make sure the next belt doesnt have an object too close
				bool move = true;
				if (getNextObject() != NULL && getNextObject()->getObjectNumMoves().size() != 0) {
					int spot = (int)getNextObject()->getMinSpaceBetween();
					if (getNextObject()->GetTile()->GetType() == TileTypeTurnBelt || getNextObject()->GetTile()->GetType() == TileTypeTurnBeltBackwards)
						spot += getMaxItemMoves() / 2;
					if ((getMaxItemMoves() + getNextObject()->getObjectNumMoves()[getNextObject()->getObjectNumMoves().size() - 1]) - getObjectNumMoves()[0] <= spot)
						move = false;
				}
				if (move)
					MovePaintBlob(i);
				else {
					m_LastItemMoved = false;
				}
			}
			else if (m_LastItemMoved) MovePaintBlob(i);
			else
			{
				// if move will make it too close to next then dont move
				if (getObjectNumMoves()[i - 1] - getObjectNumMoves()[i] < getMinSpaceBetween()) // then it will be too close
					m_LastItemMoved = false;
				else 
					MovePaintBlob(i);
			}
		}
	}

	// update next in chain
	if (getLastObject() != NULL)
		getLastObject()->Update();
}

std::shared_ptr<GameObject> findWorldObjectAtPos(int xPos, int yPos, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>>& worldGameObjects)
{
	if (worldGameObjects[xPos][yPos] != NULL) {
		std::shared_ptr<GameObject> obj;
		if (worldGameObjects[xPos][yPos]->GetParentObject() != NULL) { // use the parent obj
			obj = worldGameObjects[xPos][yPos]->GetParentObject();
		}
		else
			obj = worldGameObjects[xPos][yPos];
		if (obj->AllowNewItem())
			return obj;

	}
	std::unique_ptr<Belt> fake;
	return fake;
}

bool Belt::AllowNewItem(bool StartAtHalf)
{
	if (getObjectsInInv().size() == 0)
		return true;
	int spot = (int)getMinSpaceBetween();
	if (StartAtHalf/* || GetTile()->GetType() == TileTypeTurnBelt || GetTile()->GetType() == TileTypeTurnBeltBackwards*/)
		spot += getMaxItemMoves() / 2;
	if (getObjectNumMoves()[getObjectNumMoves().size() - 1] >= spot)
		return true;
	return false;
}

std::shared_ptr<GameObject> Belt::GetNextOrLastObject(bool isLastBelt, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Belt>>>& AllOtherBelts,
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>>& worldGameObjects)
{
	int xPos = (int)GetPos().x;
	int yPos = (int)GetPos().y;
	
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


		std::shared_ptr<Belt> possibleNextBelt = AllOtherBelts[xPos][yPos];
		// if there isnt a belt there 
		if (possibleNextBelt == NULL)
			return findWorldObjectAtPos(xPos, yPos, worldGameObjects);

		if (possibleNextBelt != NULL && possibleNextBelt->GetTile()->GetType() != TileTypeTurnBelt && possibleNextBelt->GetTile()->GetType() != TileTypeTurnBeltBackwards)
			if (possibleNextBelt->GetDirection() != GetDirection())
				return findWorldObjectAtPos(xPos, yPos, worldGameObjects);
	}
	else // if we are a turn belt and we are looking for the last belt
	{
		Direction wantedDirection = DirectionUp;
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

		std::shared_ptr<Belt> possibleNextBelt = AllOtherBelts[xPos][yPos];
		// if there isnt a belt there 
		if (possibleNextBelt == NULL)
			return findWorldObjectAtPos(xPos, yPos, worldGameObjects);

		if (possibleNextBelt == NULL && possibleNextBelt->GetDirection() != wantedDirection)
			return findWorldObjectAtPos(xPos, yPos, worldGameObjects);
		
	}

	// check if other belt already has a connected belt
	if (isLastBelt)
		if (AllOtherBelts[xPos][yPos]->getNextObject() != NULL && !OnSamePos(AllOtherBelts[xPos][yPos]->getNextObject().get()[0]))
			return findWorldObjectAtPos(xPos, yPos, worldGameObjects);
	if (!isLastBelt) {
		if (AllOtherBelts[xPos][yPos]->getLastObject() != NULL && !OnSamePos(AllOtherBelts[xPos][yPos]->getLastObject().get()[0]))
			return findWorldObjectAtPos(xPos, yPos, worldGameObjects);
	}
	
	

	return AllOtherBelts[xPos][yPos];
}


void Belt::MovePaintBlob(int pos)
{
	m_LastItemMoved = true;
	getObjectNumMoves()[pos]++;
	Vec3 moveTo = { 0,0,1 };
	// move by direction and speed
	float offset;
	if (GetDirection() == DirectionUp)
		moveTo.y = BELT_SPEED * (GetBeltTypeSpeed());
	else if (GetDirection() == DirectionDown)
		moveTo.y = -1 * BELT_SPEED * (GetBeltTypeSpeed());
	else if (GetDirection() == DirectionRight)
		moveTo.x = BELT_SPEED * (GetBeltTypeSpeed());
	else if (GetDirection() == DirectionLeft)
		moveTo.x = -1 * BELT_SPEED * (GetBeltTypeSpeed());
	

	// move to middle of the belt on turns 
	float toCheck = .5;
	if (GetDirection() == DirectionUp || GetDirection() == DirectionDown)
	{
		offset = round((getObjectsInInv()[pos]->GetPos().x - int(GetPos().x)) * 100) / 100;
		if (offset < toCheck)
			moveTo.x = BELT_SPEED * (GetBeltTypeSpeed());
		else if (offset > toCheck)
			moveTo.x = -1 * BELT_SPEED * (GetBeltTypeSpeed());
	}
	else
	{
		offset = round((getObjectsInInv()[pos]->GetPos().y - int(GetPos().y)) * 100) / 100;
		if (offset < toCheck)
			moveTo.y = BELT_SPEED * (GetBeltTypeSpeed());
		else if (offset > toCheck)
			moveTo.y = -1 * BELT_SPEED * (GetBeltTypeSpeed());
	}

	getObjectsInInv()[pos]->SetPos(getObjectsInInv()[pos]->GetPos() + moveTo);

}