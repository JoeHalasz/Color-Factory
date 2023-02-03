#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

enum TileType
{
	TileTypeBackground,
	TileTypeBackgroundDark,
	TileTypeStraightBelt,
	TileTypeTurnBelt,
	TileTypeYellowArrow,
	TileTypeOrangeArrow,
	TileTypeRedArrow,
};

enum Direction
{
	DirectionUp=0,
	DirectionRight=1,
	DirectionDown=2,
	DirectionLeft=3,
};

class WorldTile
{
private:
	TileType m_Type;
	glm::vec3 m_WorldPos;
	int m_Size;
	int m_ConnectedCount = 0;
	Direction m_Direction = DirectionUp;

public:
	WorldTile(TileType type, glm::vec3 pos, int size, Direction direction);
	WorldTile(int type, glm::vec3 pos, int size, Direction direction);
	
	bool operator==(const WorldTile& other);

	inline TileType GetType() const { return m_Type; }
	inline TileType SetType(TileType type) { m_Type = type; }
	inline glm::vec3 GetPos() const { return m_WorldPos; }
	inline int GetSize() const { return m_Size; }
	inline int GetConnectedCount() const { return m_ConnectedCount; }
	inline void UpdateConnectedCount(int udpate)  { m_ConnectedCount += udpate; }
	inline Direction GetDirection() { return m_Direction; }
	inline Direction SetDirection(Direction direction) { m_Direction = direction; }
};