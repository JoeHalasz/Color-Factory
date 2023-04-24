#include "IndoorArea.h"
#include "HelperFunctions.h"

IndoorArea::IndoorArea()
	: m_MiddlePosition(Vec3{ 0, 0, 0 }), m_DirectionLeftFromMiddle(20), m_DirectionRightFromMiddle(20), m_DirectionUpFromMiddle(20), m_DirectionDownFromMiddle(20)
{
}

IndoorArea::IndoorArea(Vec3 MiddlePosition, Direction lastAreaDirection, 
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<TruckNode>>>& AllTruckNodes, 
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<TruckStop>>>& AllTruckStops,
	bool isFirst, int DirectionLeftFromMiddle, int DirectionRightFromMiddle, int DirectionUpFromMiddle, int DirectionDownFromMiddle)
	: m_MiddlePosition(MiddlePosition), m_DirectionLeftFromMiddle(DirectionLeftFromMiddle), m_DirectionRightFromMiddle(DirectionRightFromMiddle), 
	m_DirectionUpFromMiddle(DirectionUpFromMiddle), m_DirectionDownFromMiddle(DirectionDownFromMiddle)
{
	if (!isFirst) {
		switch (lastAreaDirection)
		{
		case(DirectionUp): m_DirectionsToOtherAreas.push_back(DirectionDown); break;
		case(DirectionDown): m_DirectionsToOtherAreas.push_back(DirectionUp); break;
		case(DirectionLeft): m_DirectionsToOtherAreas.push_back(DirectionRight); break;
		case(DirectionRight): m_DirectionsToOtherAreas.push_back(DirectionLeft); break;
		default: break;
		}
	}

	std::shared_ptr<TruckNode> topLeft = GetTAtPos<TruckNode>(m_MiddlePosition.x - m_DirectionLeftFromMiddle - 3, m_MiddlePosition.y + m_DirectionUpFromMiddle + 3, AllTruckNodes);
	if (topLeft == NULL)
		topLeft = std::make_shared<TruckNode>(Vec3{ m_MiddlePosition.x - m_DirectionLeftFromMiddle - 3, m_MiddlePosition.y + m_DirectionUpFromMiddle + 3, m_MiddlePosition.z });
	std::shared_ptr<TruckNode> topRight = GetTAtPos<TruckNode>(m_MiddlePosition.x + m_DirectionRightFromMiddle + 3, m_MiddlePosition.y + m_DirectionUpFromMiddle + 3, AllTruckNodes);
	if (topRight == NULL)
		topRight = std::make_shared<TruckNode>(Vec3{ m_MiddlePosition.x + m_DirectionRightFromMiddle + 3, m_MiddlePosition.y + m_DirectionUpFromMiddle + 3, m_MiddlePosition.z });
	std::shared_ptr<TruckNode> bottomLeft = GetTAtPos<TruckNode>(m_MiddlePosition.x - m_DirectionLeftFromMiddle - 3, m_MiddlePosition.y - m_DirectionDownFromMiddle - 3, AllTruckNodes);
	if (bottomLeft == NULL)
		bottomLeft = std::make_shared<TruckNode>(Vec3{ m_MiddlePosition.x - m_DirectionLeftFromMiddle - 3, m_MiddlePosition.y - m_DirectionDownFromMiddle - 3, m_MiddlePosition.z });
	std::shared_ptr<TruckNode> bottomRight = GetTAtPos<TruckNode>(m_MiddlePosition.x + m_DirectionRightFromMiddle + 3, m_MiddlePosition.y - m_DirectionDownFromMiddle - 3, AllTruckNodes);
	if (bottomRight == NULL)
		bottomRight = std::make_shared<TruckNode>(Vec3{ m_MiddlePosition.x + m_DirectionRightFromMiddle + 3, m_MiddlePosition.y - m_DirectionDownFromMiddle - 3, m_MiddlePosition.z });


	topLeft->addConnectedNode(topRight);
	topLeft->addConnectedNode(bottomLeft);

	topRight->addConnectedNode(topLeft);
	topRight->addConnectedNode(bottomRight);

	bottomLeft->addConnectedNode(topLeft);
	bottomLeft->addConnectedNode(bottomRight);

	bottomRight->addConnectedNode(topRight);
	bottomRight->addConnectedNode(bottomLeft);

	AllTruckNodes[(int)std::floor(m_MiddlePosition.x - m_DirectionLeftFromMiddle - 3)][(int)std::floor(m_MiddlePosition.y + m_DirectionUpFromMiddle + 3)] = topLeft;
	AllTruckNodes[(int)std::floor(m_MiddlePosition.x + m_DirectionRightFromMiddle + 3)][(int)std::floor(m_MiddlePosition.y + m_DirectionUpFromMiddle + 3)] = topRight;
	AllTruckNodes[(int)std::floor(m_MiddlePosition.x - m_DirectionLeftFromMiddle - 3)][(int)std::floor(m_MiddlePosition.y - m_DirectionDownFromMiddle - 3)] = bottomLeft;
	AllTruckNodes[(int)std::floor(m_MiddlePosition.x + m_DirectionRightFromMiddle + 3)][(int)std::floor(m_MiddlePosition.y - m_DirectionDownFromMiddle - 3)] = bottomRight;
	
	// create truck stops on the left and right walls and truck nodes for those stops
	std::shared_ptr<TruckNode> leftNode = std::make_shared<TruckNode>(Vec3{ m_MiddlePosition.x - m_DirectionLeftFromMiddle, m_MiddlePosition.y, m_MiddlePosition.z }, true);
	leftNode->addConnectedNode(topLeft);
	topLeft->addConnectedNode(leftNode);
	std::shared_ptr<TruckStop> leftStop = std::make_shared<TruckStop>(leftNode);

	std::shared_ptr<TruckNode> rightNode = std::make_shared<TruckNode>(Vec3{ m_MiddlePosition.x + m_DirectionRightFromMiddle, m_MiddlePosition.y, m_MiddlePosition.z }, true);
	rightNode->addConnectedNode(bottomRight);
	bottomRight->addConnectedNode(rightNode);
	std::shared_ptr<TruckStop> rightStop = std::make_shared<TruckStop>(rightNode);

	// add the truck stops
	AllTruckStops[(int)std::floor(m_MiddlePosition.x - m_DirectionLeftFromMiddle)][(int)std::floor(m_MiddlePosition.y)] = leftStop;
	AllTruckStops[(int)std::floor(m_MiddlePosition.x + m_DirectionRightFromMiddle)][(int)std::floor(m_MiddlePosition.y)] = rightStop;

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