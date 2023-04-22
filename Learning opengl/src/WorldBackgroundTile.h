#pragma once
#include "WorldTile.h"
#include "GameObject.h"

class WorldBackgroundTile
{
private:
	TileType m_TileType = TileTypeGrass;
	Direction m_Direction = DirectionUp;
public:
	WorldBackgroundTile() {}
	WorldBackgroundTile(TileType type, Direction direction);
	inline TileType GetTileType() const { return m_TileType; }
	inline void SetTileType(TileType Type) { m_TileType = Type; }
	inline Direction GetDirection() const { return m_Direction; }
	inline void SetDirection(Direction Direction) { m_Direction = Direction; }

};