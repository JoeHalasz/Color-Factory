#include "PaintBlobCombiner.h"

PaintBlobCombiner::PaintBlobCombiner(Vec3 pos, int size, Direction direction)
	:m_Pos1(pos), m_Size(size), m_Direction(direction)
{
	m_Tile1 = WorldTile(TileTypePaintBlobContainer1);
	m_Tile2 = WorldTile(TileTypePaintBlobContainer2);
	if (direction == DirectionUp)
		m_Pos2 = pos + Vec3{ 1,0,0 };
	else if (direction == DirectionDown)
		m_Pos2 = pos + Vec3{ -1,0,0 };
	else if (direction == DirectionLeft)
		m_Pos2 = pos + Vec3{ 0, -1,0 };
	else /*if (direction == DirectionRight)*/
		m_Pos2 = pos + Vec3{ 0, 1,0 };
	
}
