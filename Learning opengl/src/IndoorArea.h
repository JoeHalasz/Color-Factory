#pragma once
#include <WorldBackgroundTile.h>
#include <unordered_map>
#include <TruckNode.h>
#include <TruckStop.h>

class AddIndoorAreaButton;

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
	IndoorArea(Vec3 MiddlePosition, Direction lastAreaDirection, 
		std::unordered_map<int, std::unordered_map<int, std::shared_ptr<TruckNode>>>& AllTruckNodes, 
		std::unordered_map<int, std::unordered_map<int, std::shared_ptr<TruckStop>>>& AllTruckStops, 
		std::unordered_map<int, std::unordered_map<int, std::shared_ptr<AddIndoorAreaButton>>>& AllButtons,
		std::vector<std::pair<IndoorArea, Direction>>& buttonMessages,
		bool isFirst = false, int DirectionLeftFromMiddle = 20, int DirectionRightFromMiddle = 20, 
		int DirectionUpFromMiddle = 20, int DirectionDownFromMiddle = 20);


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

class AddIndoorAreaButton
{
private:
	IndoorArea m_ConnectedTo;
	Direction m_Direction;
	Vec3 m_Pos;
	std::vector<std::pair<IndoorArea, Direction>> m_ButtonMessages;
public:
	AddIndoorAreaButton(IndoorArea& connectedTo, Direction direction, Vec3 pos, std::vector<std::pair<IndoorArea, Direction>>& buttonMessages)
		: m_ConnectedTo(connectedTo), m_Direction(direction), m_Pos(pos), m_ButtonMessages(buttonMessages){}
	void OnClick(){ m_ButtonMessages.push_back(std::pair<IndoorArea, Direction>(m_ConnectedTo, m_Direction));}
	inline Vec3 GetPos() const { return m_Pos; }
	inline void SetPos(Vec3 pos) { m_Pos = pos; }
};