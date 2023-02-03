#include "WorldTile.h"

WorldTile::WorldTile(TileType type, glm::vec3 pos, int size)
	:m_Type(type), m_WorldPos(pos), m_Size(size)
{
}

WorldTile::WorldTile(int type, glm::vec3 pos, int size)
	:m_Type((TileType)type), m_WorldPos(pos), m_Size(size)
{
}
