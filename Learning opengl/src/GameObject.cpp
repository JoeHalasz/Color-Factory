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
	if (lhs.GetTileCopy().GetType() == rhs.GetTileCopy().GetType() &&
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


void GameObject::MoveBy(Vec3 amount, std::unordered_map<int, std::unordered_map<int, std::vector<GameObject>>>& gameObjects)
{
	Vec3 oldWorldPos = m_WorldPos;
	m_WorldPos = m_WorldPos + amount;

	std::vector<std::vector<GameObject>> objectsNearPos;
	auto& objectsOnPos = gameObjects[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y)];
	objectsNearPos.push_back(objectsOnPos);
	objectsNearPos.push_back(gameObjects[std::floor(m_WorldPos.x + 1)][std::floor(m_WorldPos.y)]);
	objectsNearPos.push_back(gameObjects[std::floor(m_WorldPos.x - 1)][std::floor(m_WorldPos.y)]);
	objectsNearPos.push_back(gameObjects[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y + 1)]);
	objectsNearPos.push_back(gameObjects[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y - 1)]);
	auto& objectsOnOldPos = gameObjects[std::floor(oldWorldPos.x)][std::floor(oldWorldPos.y)];
	
	for (int i = 0; i < objectsNearPos.size(); i++)
	{
		for (int j = 0; j < objectsNearPos[i].size(); j++)
		{
			if (!(objectsNearPos[i][j] == this[0]))
			{
				if (checkHowClose(objectsNearPos[i][j].GetPos(), GetPos()) < .2)
				{
					m_WorldPos = oldWorldPos; // change it back if there is something too close
				}
			}
		}
	}
	
	
	if ( std::floor(oldWorldPos.x) != std::floor(m_WorldPos.x) || std::floor(oldWorldPos.y) != std::floor(m_WorldPos.y) )
	{
		for (int i = 0; i < objectsOnOldPos.size(); i++)
		{
			std::cout << objectsOnOldPos[i].GetPos().x << " " << oldWorldPos.x << "     " << objectsOnOldPos[i].GetPos().y << " " << oldWorldPos.y << std::endl;
			if (std::floor(objectsOnOldPos[i].GetPos().x) != std::floor(oldWorldPos.x) || std::floor(objectsOnOldPos[i].GetPos().y) != std::floor(oldWorldPos.y))
			{
				gameObjects[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y)].push_back(objectsOnOldPos[i]);
				gameObjects[std::floor(oldWorldPos.x)][std::floor(oldWorldPos.y)].erase(objectsOnOldPos.begin() + i);
				i--;
			}
		}
	}
}