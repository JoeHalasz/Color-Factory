#pragma once
#include <vector>
#include "WorldTile.h"
#include "PaintBlob.h"


class PaintBlobCombiner : public GameObject
{
private:
	
public:
	PaintBlobCombiner(Vec3 pos, int size, Direction direction=DirectionUp, 
		bool isBasePart = true, TileType type = TileTypePaintBlobContainer1);
	void Update();
	void Render();
	bool AllowNewItem(bool StartAtHalf = false);
};