#include "PaintBlobCombiner.h"

PaintBlobCombiner::PaintBlobCombiner(Vec3 pos, int size, Direction direction, bool isBasePart, TileType type)
{
	SetPos(pos);
	SetSize(size);
	SetDirection(direction);
	SetIsBasePart(isBasePart);
	WorldTile tile = WorldTile(type);
	SetTile(&tile);
}

void PaintBlobCombiner::Update()
{
}

void PaintBlobCombiner::Render()
{
}

bool PaintBlobCombiner::AllowNewItem(bool StartAtHalf)
{
	return false;
}
