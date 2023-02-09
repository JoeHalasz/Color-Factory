#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "MathStuff.h"

enum TileType
{
	TileTypePaintBlob,
	TileTypeBackground,
	TileTypeBackgroundDark,
	TileTypeStraightBelt,
	TileTypeTurnBelt,
	TileTypeTurnBeltBackwards,
	TileTypeYellowArrow,
	TileTypeOrangeArrow,
	TileTypeRedArrow,
};


class WorldTile
{
private:
	TileType m_TileType;
	int m_ConnectedCount = 0;
	Vec4 m_Color;

public:
	WorldTile(TileType type);
	WorldTile(TileType type, Vec4 color);
	
	inline TileType		GetType() const { return m_TileType; }
	inline void			SetType(TileType Type) { m_TileType = Type; }

	inline int			GetConnectedCount() const { return m_ConnectedCount; }
	inline void			UpdateConnectedCount(int udpate)  { m_ConnectedCount += udpate; }
	
	inline Vec4			GetColor() const { return m_Color; }


};