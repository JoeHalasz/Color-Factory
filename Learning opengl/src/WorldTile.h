#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

enum TileType
{
	TileTypeBackground,
	TileTypeBackgroundDark,
	TileTypeSingleBelt,
	TileTypeOneConnectBelt,
	TileTypeTwoConnectBelt,
	TileTypeTurnBelt,
	TileTypeYellowArrow,
	TileTypeOrangeArrow,
	TileTypeRedArrow,
};

class WorldTile
{
private:
	TileType m_Type;
	glm::vec3 m_WorldPos;
	int m_Size;
public:
	WorldTile(TileType type, glm::vec3 pos, int size);
	WorldTile(int type, glm::vec3 pos, int size);
	inline TileType GetType() { return m_Type; }
	inline glm::vec3 GetPos() { return m_WorldPos; }
	inline int GetSize() { return m_Size; }
};