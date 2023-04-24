#include "WorldTile.h"

WorldTile::WorldTile(TileType type)
	:m_TileType(type), m_Color({ 0, 0, 0, 0 })
{
}

WorldTile::WorldTile(TileType type, Vec4 color)
	:m_TileType(type), m_Color(color)
{
}


const char* WorldTile::TileTypeToString(TileType type)
{
	switch (type)
	{
	case(TileTypePaintBlob):				return "paint blob/paint blob.png";
	case(TileTypePaintBlobShading):			return "paint blob/paint blob shading.png";
	case(TileTypeBackgroundIndoor):			return "background/floortile dark.png";
	case(TileTypeStraightBelt):				return "belts/straight belt.png";
	case(TileTypeTurnBelt):					return "belts/turn belt.png";
	case(TileTypeTurnBeltBackwards):		return "belts/turn belt backwards.png";
	case(TileTypeYellowArrow):				return "belts/yellow arrow.png";
	case(TileTypeOrangeArrow):				return "belts/orange arrow.png";
	case(TileTypeRedArrow):					return "belts/red arrow.png";
	case(TileTypePaintBlobContainer1):		return "machines/PaintBlobCombiner1.png";
	case(TileTypePaintBlobContainer2):		return "machines/PaintBlobCombiner2.png";
	case(TileTypeDrawerBackground):			return "drawer/drawer background.png";
	case(TileTypeDrawerBackgroundSelected): return "drawer/drawer background selected.png";
	case(TileTypeRoad):						return "background/road.png";
	case(TileTypeRoadMiddle):				return "background/road middle.png";
	case(TileTypeGrass):					return "background/grass.png";
	case(TileTypeWall):						return "background/wall.png";
	case(TileTypeWallCorner):				return "background/wall corner.png";
	case(TileTypeWallCornerBackwards):		return "background/wall corner backwards.png";
	case(TileTypeTruck):					return "truck/truck.png";
	default:								return "";
	}
}