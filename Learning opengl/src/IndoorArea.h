#pragma once
#include <WorldBackgroundTile.h>



class IndoorArea
{
private:

	Vec3 m_MiddlePosition;
	int m_DirectionLeftFromMiddle;
	int m_DirectionRightFromMiddle;
	int m_DirectionUpFromMiddle;
	int m_DirectionDownFromMiddle;

	// if a direction is included here that means you cannot expand in that direction because there is already an area there
	std::vector<Direction> m_DirectionsToOtherAreas;

public:
	IndoorArea();
	IndoorArea(Vec3 MiddlePosition, Direction lastAreaDirection, bool isFirst = false, int DirectionLeftFromMiddle = 50, int DirectionRightFromMiddle = 50, int DirectionUpFromMiddle = 50, int DirectionDownFromMiddle = 50);
	inline Vec3 GetMiddlePosition() const { return m_MiddlePosition; }
	inline void SetMiddlePosition(Vec3 MiddlePosition) { m_MiddlePosition = MiddlePosition; }
	inline int GetDirectionLeftFromMiddle() const { return m_DirectionLeftFromMiddle; }
	inline void SetDirectionLeftFromMiddle(int DirectionLeftFromMiddle) { m_DirectionLeftFromMiddle = DirectionLeftFromMiddle; }
	inline int GetDirectionRightFromMiddle() const { return m_DirectionRightFromMiddle; }
	inline void SetDirectionRightFromMiddle(int DirectionRightFromMiddle) { m_DirectionRightFromMiddle = DirectionRightFromMiddle; }
	inline int GetDirectionUpFromMiddle() const { return m_DirectionUpFromMiddle; }
	inline void SetDirectionUpFromMiddle(int DirectionUpFromMiddle) { m_DirectionUpFromMiddle = DirectionUpFromMiddle; }
	inline int GetDirectionDownFromMiddle() const { return m_DirectionDownFromMiddle; }
	inline void SetDirectionDownFromMiddle(int DirectionDownFromMiddle) { m_DirectionDownFromMiddle = DirectionDownFromMiddle; }

	bool checkExpandAvailable(Direction direction);
	inline void AddDirectionToOtherAreas(Direction direction) { m_DirectionsToOtherAreas.push_back(direction); }


};