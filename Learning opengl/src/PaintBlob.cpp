#include "PaintBlob.h"


PaintBlob::PaintBlob(Vec3 pos, int size, Vec4 color, Direction direction)
	:m_WorldPos(pos), m_Size(size), m_Direction(direction)
{
	m_Tile = WorldTile(TileTypePaintBlob, color);
}

PaintBlob::~PaintBlob()
{
}

bool PaintBlob::Update()
{
	m_WorldPos.x += m_Speed.x;
	m_WorldPos.y += m_Speed.y;
	m_WorldPos.z += m_Speed.z;
	return false;
}

bool equals(const PaintBlob& lhs, const PaintBlob& rhs)
{
	if (lhs.GetTileCopy().GetType() == rhs.GetTileCopy().GetType() &&
		lhs.GetPosConst().x == rhs.GetPosConst().x &&
		lhs.GetPosConst().y == rhs.GetPosConst().y &&
		lhs.GetPosConst().z == rhs.GetPosConst().z &&
		lhs.GetSize() == rhs.GetSize())
	{
		return true;
	}
	return false;
}

bool PaintBlob::operator==(const PaintBlob& other)
{
	return equals(*this, other);
}

bool PaintBlob::MoveBy(Vec3 amount, std::unordered_map<int, std::unordered_map<int, std::vector<PaintBlob>>>& PaintBlobs)
{
	Vec3 oldWorldPos = m_WorldPos;

	m_WorldPos = m_WorldPos + amount;
	m_WorldPos = roundVec3(m_WorldPos);

	bool movedSquares = std::floor(oldWorldPos.x) != std::floor(m_WorldPos.x) || std::floor(oldWorldPos.y) != std::floor(m_WorldPos.y);

	std::vector<std::vector<PaintBlob>> objectsNearPos;
	auto& objectsOnPos = PaintBlobs[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y)];
	if (!movedSquares)
		objectsNearPos.push_back(objectsOnPos);
	objectsNearPos.push_back(PaintBlobs[std::floor(m_WorldPos.x + 1)][std::floor(m_WorldPos.y)]);
	objectsNearPos.push_back(PaintBlobs[std::floor(m_WorldPos.x - 1)][std::floor(m_WorldPos.y)]);
	objectsNearPos.push_back(PaintBlobs[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y + 1)]);
	objectsNearPos.push_back(PaintBlobs[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y - 1)]);
	auto& objectsOnOldPos = PaintBlobs[std::floor(oldWorldPos.x)][std::floor(oldWorldPos.y)];
	

	for (int i = 0; i < objectsNearPos.size(); i++)
	{
		for (int j = 0; j < objectsNearPos[i].size(); j++)
		{
			if (!(objectsNearPos[i][j] == this[0]))
			{
				if (checkHowClose(objectsNearPos[i][j].GetPos(), GetPos()) < .5)
				{
					m_WorldPos = oldWorldPos; // change it back if there is something too close
					return false;
				}
			}
		}
	}
	
	
	if (movedSquares)
	{
		for (int i = 0; i < objectsOnOldPos.size(); i++)
		{
			if (std::floor(objectsOnOldPos[i].GetPos().x) != std::floor(oldWorldPos.x) || std::floor(objectsOnOldPos[i].GetPos().y) != std::floor(oldWorldPos.y))
			{
				PaintBlobs[std::floor(m_WorldPos.x)][std::floor(m_WorldPos.y)].push_back(objectsOnOldPos[i]);
				PaintBlobs[std::floor(oldWorldPos.x)][std::floor(oldWorldPos.y)].erase(objectsOnOldPos.begin() + i);
				i--;
			}
		}
	}
	return true;
}

void PaintBlob::CombineColor(glm::vec4 otherColor)
{

}

glm::vec4 PaintBlob::ConvertToRGB()
{
	glm::vec4 RGBValues;
	RGBValues.a = 1;
	RGBValues.r = (1 - m_Tile.GetColor().c) * (1 - m_Tile.GetColor().k);
	RGBValues.g = (1 - m_Tile.GetColor().m) * (1 - m_Tile.GetColor().k);
	RGBValues.b = (1 - m_Tile.GetColor().y) * (1 - m_Tile.GetColor().k);
	return RGBValues;
}
