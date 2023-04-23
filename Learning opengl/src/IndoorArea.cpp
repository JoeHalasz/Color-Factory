#include "IndoorArea.h"

IndoorArea::IndoorArea()
	: m_MiddlePosition(Vec3{ 0, 0, 0 }), m_DirectionLeftFromMiddle(20), m_DirectionRightFromMiddle(20), m_DirectionUpFromMiddle(20), m_DirectionDownFromMiddle(20)
{
}

IndoorArea::IndoorArea(Vec3 MiddlePosition, Direction lastAreaDirection, bool isFirst, int DirectionLeftFromMiddle, int DirectionRightFromMiddle, int DirectionUpFromMiddle, int DirectionDownFromMiddle)
	: m_MiddlePosition(MiddlePosition), m_DirectionLeftFromMiddle(DirectionLeftFromMiddle), m_DirectionRightFromMiddle(DirectionRightFromMiddle), 
	m_DirectionUpFromMiddle(DirectionUpFromMiddle), m_DirectionDownFromMiddle(DirectionDownFromMiddle)
{
	if (isFirst) return;
	switch (lastAreaDirection)
	{
		case(DirectionUp): m_DirectionsToOtherAreas.push_back(DirectionDown); break;
		case(DirectionDown): m_DirectionsToOtherAreas.push_back(DirectionUp); break;
		case(DirectionLeft): m_DirectionsToOtherAreas.push_back(DirectionRight); break;
		case(DirectionRight): m_DirectionsToOtherAreas.push_back(DirectionLeft); break;
		default: break;
	}
}



bool IndoorArea::checkExpandAvailable(Direction direction) {
	// if this direction is in the list of directions to other areas then it is not available
	for (Direction dir : m_DirectionsToOtherAreas) {
		if (dir == direction) {
			return false;
		}
	}
	return true;
}