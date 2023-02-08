#include "WorldTile.h"

WorldTile::WorldTile(TileType type)
	:m_TileType(type), m_Color({ 0, 0, 0, 0 })
{
}

WorldTile::WorldTile(TileType type, Vec4 color)
	:m_TileType(type), m_Color(color)
{
}
