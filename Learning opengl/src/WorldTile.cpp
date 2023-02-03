#include "WorldTile.h"

WorldTile::WorldTile(TileType type, glm::vec3 pos, int size, Direction direction)
	:m_Type(type), m_WorldPos(pos), m_Size(size), m_Direction(direction)
{
}

WorldTile::WorldTile(int type, glm::vec3 pos, int size, Direction direction)
	:m_Type((TileType)type), m_WorldPos(pos), m_Size(size), m_Direction(direction)
{
}

bool equals(const WorldTile& lhs, const WorldTile& rhs)
{
	if (lhs.GetType() == rhs.GetType() &&
		lhs.GetPos().x == rhs.GetPos().x &&
		lhs.GetPos().y == rhs.GetPos().y &&
		lhs.GetPos().z == rhs.GetPos().z &&
		lhs.GetSize() == rhs.GetSize())
	{
		return true;
	}
	return false;
}

bool WorldTile::operator==(const WorldTile& other)
{
	return equals(*this, other);
}


