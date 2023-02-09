#include "Belt.h"

Belt::Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType)
	:GameObject(tile, pos, size, direction), m_BeltType(beltType)
{
	if (beltType == BeltTypeYellow) m_ArrowTile = TileType(TileTypeYellowArrow);
	if (beltType == BeltTypeOrange) m_ArrowTile = TileType(TileTypeOrangeArrow);
	if (beltType == BeltTypeRed) m_ArrowTile = TileType(TileTypeRedArrow);
	
}

void Belt::Update(std::unordered_map<int, std::unordered_map<int, std::vector<GameObject>>>& gameObjects)
{	// move all the paint blobs by the belt speed
	std::vector<GameObject>& ObjectsOnSameSquare = gameObjects[std::floor(GetPos().x)][std::floor(GetPos().y)];
	for (int i = 0; i < ObjectsOnSameSquare.size(); i++)
	{
		if (ObjectsOnSameSquare[i].GetTile()->GetType() == TileTypePaintBlob)
		{
			float xChange = 0, yChange = 0, xFix = 0, yFix = 0, direction = 0;
			if (GetDirection() == DirectionUp) {
				ObjectsOnSameSquare[i].MoveBy(Vec3{ 0, BELT_SPEED * (m_BeltType + 1), 0 }, gameObjects);
			}
			else if (GetDirection() == DirectionDown) {
				ObjectsOnSameSquare[i].MoveBy(Vec3{ 0, -1 * BELT_SPEED * (m_BeltType + 1), 0 }, gameObjects);
			}
			else if (GetDirection() == DirectionRight) {
				ObjectsOnSameSquare[i].MoveBy(Vec3{BELT_SPEED * (m_BeltType + 1), 0, 0 }, gameObjects);
			}
			else if (GetDirection() == DirectionLeft) {
				ObjectsOnSameSquare[i].MoveBy(Vec3{ -1 * BELT_SPEED * (m_BeltType + 1), 0, 0 }, gameObjects);
			}
		}
	}
	
	// move the arrow
	/*if (GetDirection() == DirectionUp) m_ArrowOffset.y += m_ArrowSpeed;
	else if (GetDirection() == DirectionDown) m_ArrowOffset.y -= m_ArrowSpeed;
	else if (GetDirection() == DirectionRight) m_ArrowOffset.x += m_ArrowSpeed;
	else if (GetDirection() == DirectionLeft) m_ArrowOffset.x -= m_ArrowSpeed;*/
}

Vec3 Belt::GetArrowPos() const
{
	return Vec3{
		GameObject::GetPos().x + m_ArrowOffset.x,
		GameObject::GetPos().y + m_ArrowOffset.y,
		GameObject::GetPos().z + +m_ArrowOffset.z
	};
}
