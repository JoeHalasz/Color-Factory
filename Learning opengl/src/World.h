#pragma once

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

#include "Input.h"

#include "PaintBlob.h"
#include "Belt.h"
#include <memory>
#include <cmath>

#include "PaintBlobCombiner.h"
#include "WorldBackgroundTile.h"
#include "IndoorArea.h"
#include "Truck.h"

#include "HelperFunctions.h"


class IndoorAreaAddButton;

class World
{
private:
	GLFWwindow* m_Window;
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	double m_ZoomAmount = 0;
	int m_BlockSize = 75;

	float m_Rotation = 0;
	double ZOOM_MAX = 25;
	double ZOOM_MIN = 1;

	int m_MousePosX = 0;
	int m_MousePosY = 0;

	Input* m_Input;

	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<PaintBlob>>> m_PaintBlobs;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Belt>>> m_Belts;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>> m_GameObjects;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<WorldBackgroundTile>>> m_WorldBackgroundTiles;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Truck>>> m_Trucks;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<TruckStop>>> m_TruckStops;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<TruckNode>>> m_TruckNodes;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<IndoorAreaAddButton>>> m_IndoorAreaAddButtons;

	// TODO this should be like the ones above so that indoor areas cant be placed on top of each other. Current logic allows for that sometimes.
	std::vector<std::shared_ptr<IndoorArea>> m_IndoorAreas;

	bool AddBelt(BeltType beltColor, Vec3 pos, Direction direction);
	bool AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size);
	bool AddTruck();
	bool AddPaintBlobCombiner(Vec3 pos, Direction direction, int numInputs);
	bool CheckAddBackgroundTile(Vec3 pos);

public:
	bool IS3D = false;
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	~World();
	void OnUpdate();
	bool AddPaintBlob(std::shared_ptr<PaintBlob> newObject);
	bool BeltCanBeMade(Vec3 pos, BeltType beltColor, Direction direction);
	bool NothingAtPos(Vec3 pos);
	void DeleteAllAtPos(Vec3 pos);
	int GetBeltDirectionAt(int x, int y);
	bool HasIndoorAreaInDirection(std::shared_ptr<IndoorArea> area, Direction direction);
	bool AddIndoorArea(std::shared_ptr <IndoorArea> lastArea, Direction directionToCreate, bool isFirst=false);

	std::shared_ptr<Belt> GetBeltAtPos(float x, float y) { return GetTAtPos<Belt>(x, y, m_Belts); }
	std::shared_ptr<PaintBlob> GetPaintBlobAtPos(float x, float y) { return GetTAtPos<PaintBlob>(x, y, m_PaintBlobs); }
	std::shared_ptr<GameObject> GetGameObjectAtPos(float x, float y) { return GetTAtPos<GameObject>(x, y, m_GameObjects); }
	std::shared_ptr<WorldBackgroundTile> GetWorldBackgroundTileAtPos(float x, float y) { return GetTAtPos<WorldBackgroundTile>(x, y, m_WorldBackgroundTiles); }
	std::shared_ptr<Truck> GetTruckAtPos(float x, float y) { return GetTAtPos<Truck>(x, y, m_Trucks); }
	std::shared_ptr<TruckStop> GetTruckStopAtPos(float x, float y) { return GetTAtPos<TruckStop>(x, y, m_TruckStops); }
	std::shared_ptr<TruckNode> GetTruckNodeAtPos(float x, float y) { return GetTAtPos<TruckNode>(x, y, m_TruckNodes); }
	std::shared_ptr<IndoorAreaAddButton> GetIndoorAreaAddButtonAtPos(float x, float y, bool printIt=false) { return GetTAtPos<IndoorAreaAddButton>(x, y, m_IndoorAreaAddButtons, printIt); }


	inline glm::vec3 GetPosition() const { return m_Position; }
	inline int GetZoomAmount() const { return (int)m_ZoomAmount;  }
	inline float GetRotation() const { return m_Rotation;  }
	inline int GetBlockSize() { return m_BlockSize; }
	inline int GetMousePosX() { return m_MousePosX; }
	inline int GetMousePosY() { return m_MousePosY; }
	inline void SetInput() { m_Input = new Input(m_Window, IS3D);}
	inline Input* GetInput() { return m_Input; }

	inline void AddPaintBlobAtPos(std::shared_ptr<PaintBlob> newObject, float x, float y) { m_PaintBlobs[(int)std::floor(x)][(int)std::floor(y)] = newObject; }
	inline void AddBeltAtPos(std::shared_ptr <Belt> newBelt, float x, float y) { m_Belts[(int)std::floor(x)][(int)std::floor(y)] = newBelt; }
	inline void AddGameObjectAtPos(std::shared_ptr <GameObject> newObject, float x, float y) { m_GameObjects[(int)std::floor(x)][(int)std::floor(y)] = newObject; }
	inline void AddWorldBackgroundTileAtPos(std::shared_ptr <WorldBackgroundTile> newObject, float x, float y) { m_WorldBackgroundTiles[(int)std::floor(x)][(int)std::floor(y)] = newObject; }
	inline void AddTruckAtPos(std::shared_ptr <Truck> newObject, float x, float y) { m_Trucks[(int)std::floor(x)][(int)std::floor(y)] = newObject; }
	inline void AddTruckStopAtPos(std::shared_ptr <TruckStop> newObject, float x, float y) { m_TruckStops[(int)std::floor(x)][(int)std::floor(y)] = newObject; }
	inline void AddTruckNodeAtPos(std::shared_ptr <TruckNode> newObject, float x, float y) { m_TruckNodes[(int)std::floor(x)][(int)std::floor(y)] = newObject; }
	inline void AddIndoorAreaAddButtonAtPos(std::shared_ptr <IndoorAreaAddButton> newObject, float x, float y) { m_IndoorAreaAddButtons[(int)std::floor(x)][(int)std::floor(y)] = newObject; }


	inline bool isIndoors(Vec3 pos)
	{
		if (GetWorldBackgroundTileAtPos(pos.x, pos.y) == NULL || GetWorldBackgroundTileAtPos(pos.x, pos.y)->GetTileType() != TileTypeBackgroundIndoor)
			return false; 
		return true;
	}
	inline std::vector<std::shared_ptr<IndoorArea>> GetIndoorAreas() { return m_IndoorAreas; }
	inline void DeleteIndoorAreaAddButtonAtPos(float x, float y) { m_IndoorAreaAddButtons[(int)std::floor(x)][(int)std::floor(y)] = NULL; }

};


class IndoorAreaAddButton
{
private:
	std::shared_ptr<IndoorArea> m_ConnectedTo;
	Direction m_Direction;
	Vec3 m_Pos;
	World m_World;
public:
	IndoorAreaAddButton(std::shared_ptr<IndoorArea> connectedTo, Direction direction, Vec3 pos, World& world)
		: m_ConnectedTo(connectedTo), m_Direction(direction), m_Pos(pos), m_World(world) {}
	inline void OnClick() { m_World.AddIndoorArea(m_ConnectedTo, m_Direction, false); }
	inline Vec3 GetPos() const { return m_Pos; }
	inline void SetPos(Vec3 pos) { m_Pos = pos; }
	inline std::shared_ptr<IndoorArea> GetConnectedTo(){ return m_ConnectedTo; }
	inline Direction GetDirection() { return m_Direction; }
};