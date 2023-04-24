#pragma once
#include <MathStuff.h>
#include <vector>


class TruckNode
{
private:
	std::vector<std::shared_ptr<TruckNode>> m_ConnectedNodes;
	Vec3 m_Position;
	bool m_IsStop = false;

public:
	TruckNode() : m_Position(Vec3{ 0,0,0 }) {}
	TruckNode(Vec3 position,bool isStop = false);

	std::vector<std::shared_ptr<TruckNode>> GetConnectedNodes() const { return m_ConnectedNodes; }
	inline void addConnectedNode(std::shared_ptr<TruckNode> node) { m_ConnectedNodes.push_back(node); }
	Vec3 GetPos() const { return m_Position; }

	inline bool GetIsStop() const { return m_IsStop; }
};