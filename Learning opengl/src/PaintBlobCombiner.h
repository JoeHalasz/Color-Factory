#pragma once
#include <vector>
#include "WorldTile.h"
#include "PaintBlob.h"


class PaintBlobCombiner
{
private:
	WorldTile m_Tile1;
	WorldTile m_Tile2;
	Vec3 m_Pos1;
	Vec3 m_Pos2; // pos of the second tile
	int m_Size;
	Direction m_Direction;
public:
	PaintBlobCombiner(Vec3 pos, int size, Direction direction=DirectionUp);

	inline WorldTile* GetTile1() { return &m_Tile1; }
	inline WorldTile* GetTile2() { return &m_Tile2; }
	inline Vec3 GetPos1() const { return m_Pos1; }
	inline Vec3 GetPos2() const { return m_Pos2; }
	inline int GetSize() const { return m_Size; }
	inline Direction GetDirection() const { return m_Direction; }
};