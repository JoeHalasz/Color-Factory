#include "Belt.h"

Belt::Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType)
	:GameObject(tile, pos, size, direction), m_BeltType(beltType)
{
	if (beltType == BeltTypeYellow) m_ArrowTile = TileType(TileTypeYellowArrow);
	if (beltType == BeltTypeOrange) m_ArrowTile = TileType(TileTypeOrangeArrow);
	if (beltType == BeltTypeRed) m_ArrowTile = TileType(TileTypeRedArrow);
	
}

bool Belt::Update(std::vector<GameObject>& ObjectsOnSameSquare)
{	// move all the paint blobs by the belt speed
	bool somethingChangedPos = false;
	for (int i = 0; i < ObjectsOnSameSquare.size(); i++)
	{
		if (ObjectsOnSameSquare[i].GetTile().GetType() == TileTypePaintBlob)
		{
			if (GetDirection() == DirectionUp) somethingChangedPos = ObjectsOnSameSquare[i].ChangeOnTileOffset({ 0, BELT_SPEED * (m_BeltType + 1),0 });
			else if (GetDirection() == DirectionDown) somethingChangedPos = ObjectsOnSameSquare[i].ChangeOnTileOffset({ 0, -1 * BELT_SPEED * (m_BeltType + 1),0 });
			else if (GetDirection() == DirectionRight) somethingChangedPos = ObjectsOnSameSquare[i].ChangeOnTileOffset({BELT_SPEED * (m_BeltType + 1), 0 ,0 });
			else if (GetDirection() == DirectionLeft) somethingChangedPos = ObjectsOnSameSquare[i].ChangeOnTileOffset({ -1 * BELT_SPEED * (m_BeltType + 1), 0, 0 });
		}
	}
	
	// move the arrow
	/*if (GetDirection() == DirectionUp) m_ArrowOffset.y += m_ArrowSpeed;
	else if (GetDirection() == DirectionDown) m_ArrowOffset.y -= m_ArrowSpeed;
	else if (GetDirection() == DirectionRight) m_ArrowOffset.x += m_ArrowSpeed;
	else if (GetDirection() == DirectionLeft) m_ArrowOffset.x -= m_ArrowSpeed;*/
	return somethingChangedPos;
}

Vec3 Belt::GetArrowPos() const
{
	return Vec3{
		GameObject::GetPos().x + m_ArrowOffset.x,
		GameObject::GetPos().y + m_ArrowOffset.y,
		GameObject::GetPos().z + +m_ArrowOffset.z
	};
}
