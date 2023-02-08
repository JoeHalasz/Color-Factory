#include "GameObject.h"


GameObject::GameObject(WorldTile tile, Vec3 pos, int size)
	:m_Tile(tile), m_WorldPos(pos), m_Size(size)
{
}

GameObject::GameObject(WorldTile tile, Vec3 pos, int size, Direction direction)
	:m_Tile(tile), m_WorldPos(pos), m_Size(size), m_Direction(direction)
{
}

GameObject::~GameObject()
{
}



bool GameObject::Update()
{
	m_WorldPos.x += m_Speed.x;
	m_WorldPos.y += m_Speed.y;
	m_WorldPos.z += m_Speed.z;
	return false;
}


bool equals(const GameObject& lhs, const GameObject& rhs)
{
	if (lhs.GetTile().GetType() == rhs.GetTile().GetType() &&
		lhs.GetPos().x == rhs.GetPos().x &&
		lhs.GetPos().y == rhs.GetPos().y &&
		lhs.GetPos().z == rhs.GetPos().z &&
		lhs.GetSize() == rhs.GetSize())
	{
		return true;
	}
	return false;
}

bool GameObject::operator==(const GameObject& other)
{
	return equals(*this, other);
}


bool GameObject::ChangeOnTileOffset(Vec3 change)
{
	m_OnTileOffset = m_OnTileOffset + change;
	if (m_OnTileOffset.x > 1.0f) { m_OnTileOffset.x -= 1; m_WorldPos.x += 1; return true; }
	if (m_OnTileOffset.y > 1.0f) { m_OnTileOffset.y -= 1; m_WorldPos.y += 1; return true;}
	if (m_OnTileOffset.x < 0.0f) { m_OnTileOffset.x += 1; m_WorldPos.x -= 1; return true;}
	if (m_OnTileOffset.y < 0.0f) { m_OnTileOffset.y += 1; m_WorldPos.y -= 1; return true;}
	return false;
}