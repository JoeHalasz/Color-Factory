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
	// make a list of worldbackgroundtiles and use it on line 255 in renderer.h
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<WorldBackgroundTile>>> m_WorldBackgroundTiles;

	bool AddBelt(BeltType beltColor, Vec3 pos, Direction direction);
	bool AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size);
	bool AddPaintBlobCombiner(Vec3 pos, Direction direction, int numInputs);
	bool AddIndoorArea(IndoorArea lastArea, Direction directionToCreate, bool isFirst);
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

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline int GetZoomAmount() const { return (int)m_ZoomAmount;  }
	inline float GetRotation() const { return m_Rotation;  }
	inline int GetBlockSize() { return m_BlockSize; }
	inline int GetMousePosX() { return m_MousePosX; }
	inline int GetMousePosY() { return m_MousePosY; }
	inline void SetInput() { m_Input = new Input(m_Window, IS3D);}
	inline Input* GetInput() { return m_Input; }

	inline std::shared_ptr<PaintBlob> GetPaintBlobAtPos(float x, float y) { return m_PaintBlobs[(int)std::floor(x)][(int)std::floor(y)]; }
	inline void AddPaintBlobAtPos(std::shared_ptr<PaintBlob> newObject, float x, float y) { m_PaintBlobs[(int)std::floor(x)][(int)std::floor(y)] = newObject; }

	inline std::shared_ptr<Belt> GetBeltAtPos(float x, float y) { return m_Belts[(int)std::floor(x)][(int)std::floor(y)]; }
	inline void AddBeltAtPos(std::shared_ptr <Belt> newBelt, float x, float y) { m_Belts[(int)std::floor(x)][(int)std::floor(y)] = newBelt; }

	inline std::shared_ptr<GameObject> GetGameObjectAtPos(float x, float y) { return m_GameObjects[(int)std::floor(x)][(int)std::floor(y)]; }
	inline void AddGameObjectAtPos(std::shared_ptr <GameObject> newObject, float x, float y) { m_GameObjects[(int)std::floor(x)][(int)std::floor(y)] = newObject; }

	inline std::shared_ptr<WorldBackgroundTile> GetWorldBackgroundTileAtPos(float x, float y) { return m_WorldBackgroundTiles[(int)std::floor(x)][(int)std::floor(y)]; }
	inline void AddWorldBackgroundTileAtPos(std::shared_ptr <WorldBackgroundTile> newObject, float x, float y) { m_WorldBackgroundTiles[(int)std::floor(x)][(int)std::floor(y)] = newObject; }

};