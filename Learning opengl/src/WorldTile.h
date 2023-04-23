#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "MathStuff.h"


enum TileType
{
	TileTypePaintBlob,
	TileTypePaintBlobShading,
	TileTypeBackgroundIndoor,
	TileTypeStraightBelt,
	TileTypeTurnBelt,
	TileTypeTurnBeltBackwards,
	TileTypeYellowArrow,
	TileTypeOrangeArrow,
	TileTypeRedArrow,
	TileTypePaintBlobContainer1,
	TileTypePaintBlobContainer2,
	TileTypeDrawerBackground,
	TileTypeDrawerBackgroundSelected,
	TileTypeRoad,
	TileTypeRoadMiddle,
	TileTypeGrass,
	TileTypeWall,
	TileTypeWallCorner,
	TileTypeWallCornerBackwards,
	TileTypeNumTileTypes
};




class WorldTile
{
private:
	TileType m_TileType = TileTypePaintBlob;
	int m_ConnectedCount = 0;
	Vec4 m_Color = { 0 };

public:

	WorldTile() {}
	WorldTile(TileType type);
	WorldTile(TileType type, Vec4 color);
	
	int TOTALTILES = 17;
	static const char*  TileTypeToString(TileType type);
	inline TileType		GetType() const { return m_TileType; }
	inline void			SetType(TileType Type) { m_TileType = Type; }

	inline int			GetConnectedCount() const { return m_ConnectedCount; }
	inline void			UpdateConnectedCount(int udpate)  { m_ConnectedCount += udpate; }
	
	inline Vec4			GetColor() const { return m_Color; }
	inline void			SetColor(Vec4 color) { m_Color = color; } 

};