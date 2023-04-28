#pragma once
#include <MathStuff.h>
#include <vector>
#include "GameObject.h"
#include <utility>

class TruckNode
{
private:
	std::vector<std::pair<std::shared_ptr<TruckNode>, Direction>> m_ConnectedNodes;
	Vec3 m_Position;
	bool m_IsStop = false;
	int m_F = 0;
	int m_G = 0;
	int m_H = 0;

	std::shared_ptr<TruckNode> m_Parent = nullptr;

public:
	TruckNode() : m_Position(Vec3{ 0,0,0 }) {}
	TruckNode(Vec3 position,bool isStop = false);

	std::vector<std::pair<std::shared_ptr<TruckNode>, Direction>>& GetConnectedNodes() { return m_ConnectedNodes; }
	inline void addConnectedNode(std::shared_ptr<TruckNode> node, Direction directionToMove) { m_ConnectedNodes.push_back(std::pair<std::shared_ptr<TruckNode>, Direction>(node, directionToMove)); }
	Vec3 GetPos() const { return m_Position; }

	inline bool GetIsStop() const { return m_IsStop; }
	inline int GetF() const { return m_F; }
	inline int GetG() const { return m_G; }
	inline int GetH() const { return m_H; }
	inline void SetF(int f) { m_F = f; }
	inline void SetG(int g) { m_G = g; }
	inline void SetH(int h) { m_H = h; }
	std::shared_ptr<TruckNode> GetParentNode() const { return m_Parent; }
	void SetParentNode(std::shared_ptr<TruckNode> parent) { m_Parent = parent; }
};