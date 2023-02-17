//#include "Belt.h"
//
//Belt::Belt(WorldTile tile, Vec3 pos, int size, Direction direction, BeltType beltType)
//	:m_Tile(tile), m_WorldPos(pos), m_Size(size), m_Direction(direction), m_BeltType(beltType)
//{
//	if (beltType == BeltTypeYellow) m_ArrowTile = TileType(TileTypeYellowArrow);
//	if (beltType == BeltTypeOrange) m_ArrowTile = TileType(TileTypeOrangeArrow);
//	if (beltType == BeltTypeRed) m_ArrowTile = TileType(TileTypeRedArrow);
//	
//}
//
//void Belt::Update(std::unordered_map<int, std::unordered_map<int, std::vector<PaintBlob>>>& PaintBlobs)
//{	// move all the paint blobs by the belt speed
//	std::vector<PaintBlob>& ObjectsOnSameSquare = PaintBlobs[std::floor(GetPos().x)][std::floor(GetPos().y)];
//	float offset, direction;
//	float xChange, yChange, xFix, yFix;
//	bool worked1 = true;
//	bool worked2 = true;
//	for (int i = 0; i < ObjectsOnSameSquare.size(); i++)
//	{
//		if (ObjectsOnSameSquare[i].GetTile()->GetType() == TileTypePaintBlob)
//		{
//			Vec3 oldPos = ObjectsOnSameSquare[i].GetPos();
//			xChange = 0, yChange = 0, xFix = 0, yFix = 0, direction = 0;
//			if (GetDirection() == DirectionUp) {
//				worked1 = ObjectsOnSameSquare[i].MoveBy(Vec3{ 0, BELT_SPEED * (m_BeltType + 1), 0 }, PaintBlobs);
//				offset = ObjectsOnSameSquare[i].GetPos().x - std::floor(ObjectsOnSameSquare[i].GetPos().x);
//				direction = .5f - offset;
//				direction /= std::abs(direction);
//				if (std::abs(offset) != .5f)
//					worked2 = ObjectsOnSameSquare[i].MoveBy(Vec3{ BELT_SPEED * (m_BeltType + 1) * direction, 0, 0}, PaintBlobs);
//
//			}
//			else if (GetDirection() == DirectionDown) {
//				worked1 = ObjectsOnSameSquare[i].MoveBy(Vec3{ 0, -1 * BELT_SPEED * (m_BeltType + 1), 0 }, PaintBlobs);
//				offset = ObjectsOnSameSquare[i].GetPos().x - std::floor(ObjectsOnSameSquare[i].GetPos().x);
//				direction = .5f - offset;
//				direction /= std::abs(direction);
//				if (std::abs(offset) != .5f)
//					worked2 = ObjectsOnSameSquare[i].MoveBy(Vec3{ BELT_SPEED * (m_BeltType + 1) * direction, 0, 0 }, PaintBlobs);
//			}
//			else if (GetDirection() == DirectionRight) {
//				worked1 = ObjectsOnSameSquare[i].MoveBy(Vec3{BELT_SPEED * (m_BeltType + 1), 0, 0 }, PaintBlobs);
//				offset = ObjectsOnSameSquare[i].GetPos().y - std::floor(ObjectsOnSameSquare[i].GetPos().y);
//				direction = .5f - offset;
//				direction /= std::abs(direction);
//				if (std::abs(offset) != .5f)
//					worked2 = ObjectsOnSameSquare[i].MoveBy(Vec3{ 0, BELT_SPEED * (m_BeltType + 1) * direction, 0 }, PaintBlobs);
//			}
//			else if (GetDirection() == DirectionLeft) {
//				worked1 = ObjectsOnSameSquare[i].MoveBy(Vec3{ -1 * BELT_SPEED * (m_BeltType + 1), 0, 0 }, PaintBlobs);
//				offset = ObjectsOnSameSquare[i].GetPos().y - std::floor(ObjectsOnSameSquare[i].GetPos().y);
//				direction = .5f - offset;
//				direction /= std::abs(direction);
//				if (std::abs(offset) != .5f)
//					worked2 = ObjectsOnSameSquare[i].MoveBy(Vec3{ 0, BELT_SPEED * (m_BeltType + 1) * direction, 0 }, PaintBlobs);
//			}
//			if (!worked1 || !worked2)
//			{
//				ObjectsOnSameSquare[i].SetPos(oldPos); // if one of the moves didnt work then dont move at all
//			}
//
//		}
//	}
//	
//	// move the arrow
//	/*if (GetDirection() == DirectionUp) m_ArrowOffset.y += m_ArrowSpeed;
//	else if (GetDirection() == DirectionDown) m_ArrowOffset.y -= m_ArrowSpeed;
//	else if (GetDirection() == DirectionRight) m_ArrowOffset.x += m_ArrowSpeed;
//	else if (GetDirection() == DirectionLeft) m_ArrowOffset.x -= m_ArrowSpeed;*/
//}
//
//Vec3 Belt::GetArrowPos() const
//{
//	return Vec3{
//		GetPos().x + m_ArrowOffset.x,
//		GetPos().y + m_ArrowOffset.y,
//		GetPos().z + +m_ArrowOffset.z
//	};
//}
