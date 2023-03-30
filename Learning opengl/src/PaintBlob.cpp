#include "PaintBlob.h"


PaintBlob::PaintBlob(Vec3 pos, int size, Vec4 color, Direction direction)
{
	SetPos(pos);
	SetSize(size);
	SetDirection(direction);
	WorldTile tile = WorldTile(TileTypePaintBlob, color);
	SetTile(&tile);
}

PaintBlob::~PaintBlob()
{
}


bool equals(const PaintBlob& lhs, const PaintBlob& rhs)
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

bool PaintBlob::operator==(const PaintBlob& other)
{
	return equals(*this, other);
}

bool PaintBlob::MoveBy(Vec3 amount, std::unordered_map<int, std::unordered_map<int, std::vector<PaintBlob>>>& PaintBlobs)
{
	Vec3 oldWorldPos = GetPos();

	SetPos(GetPos() + amount);
	SetPos(roundVec3(GetPos()));

	bool movedSquares = std::floor(oldWorldPos.x) != std::floor(GetPos().x) || std::floor(oldWorldPos.y) != std::floor(GetPos().y);

	std::vector<std::vector<PaintBlob>> objectsNearPos;
	auto& objectsOnPos = PaintBlobs[(int)std::floor(GetPos().x)][(int)std::floor(GetPos().y)];
	if (!movedSquares)
		objectsNearPos.push_back(objectsOnPos);
	objectsNearPos.push_back(PaintBlobs[(int)std::floor(GetPos().x + 1)][(int)std::floor(GetPos().y)]);
	objectsNearPos.push_back(PaintBlobs[(int)std::floor(GetPos().x - 1)][(int)std::floor(GetPos().y)]);
	objectsNearPos.push_back(PaintBlobs[(int)std::floor(GetPos().x)][(int)std::floor(GetPos().y + 1)]);
	objectsNearPos.push_back(PaintBlobs[(int)std::floor(GetPos().x)][(int)std::floor(GetPos().y - 1)]);
	auto& objectsOnOldPos =  PaintBlobs[(int)std::floor(oldWorldPos.x)][(int)std::floor(oldWorldPos.y)];
	

	for (unsigned int i = 0; i < objectsNearPos.size(); i++)
	{
		for (unsigned int j = 0; j < objectsNearPos[i].size(); j++)
		{
			if (!(objectsNearPos[i][j] == this[0]))
			{
				if (checkHowClose(objectsNearPos[i][j].GetPos(), GetPos()) < .5)
				{
					SetPos(oldWorldPos); // change it back if there is something too close
					return false;
				}
			}
		}
	}
	
	
	if (movedSquares)
	{
		for (unsigned int i = 0; i < objectsOnOldPos.size(); i++)
		{
			if (std::floor(objectsOnOldPos[i].GetPos().x) != std::floor(oldWorldPos.x) || std::floor(objectsOnOldPos[i].GetPos().y) != std::floor(oldWorldPos.y))
			{
				PaintBlobs[(int)std::floor(GetPos().x)][(int)std::floor(GetPos().y)].push_back(objectsOnOldPos[i]);
				PaintBlobs[(int)std::floor(oldWorldPos.x)][(int)std::floor(oldWorldPos.y)].erase(objectsOnOldPos.begin() + i);
				i--;
			}
		}
	}
	return true;
}

Vec4 PaintBlob::ConvertToRGB()
{
	return {
		(1 - GetTile()->GetColor().c) * (1 - GetTile()->GetColor().k),
		(1 - GetTile()->GetColor().m) * (1 - GetTile()->GetColor().k),
		(1 - GetTile()->GetColor().y) * (1 - GetTile()->GetColor().k),
		1
	};
}
