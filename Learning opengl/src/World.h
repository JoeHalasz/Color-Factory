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

#include "PaintBlobCombiner.h"

class World
{
private:
	GLFWwindow* m_Window;
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_ZoomAmount = 0;
	int m_BlockSize = 75;

	float m_Rotation = 0;
	float ZOOM_MAX = 25;
	float ZOOM_MIN = 1;

	int m_MousePosX = 0;
	int m_MousePosY = 0;

	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<PaintBlob>>> m_PaintBlobs;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Belt>>> m_Belts;
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<GameObject>>> m_GameObjects;

	bool AddBelt(BeltType beltColor, Vec3 pos, Direction direction);
	bool AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size);
	bool AddPaintBlobCombiner(Vec3 pos, Direction direction, int numInputs);

public:
	bool IS3D = false; // Only works if there is only one texture
	
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	~World();
	void OnUpdate(Input* input);
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

	inline std::shared_ptr<PaintBlob> GetPaintBlobAtPos(float x, float y) { return m_PaintBlobs[std::floor(x)][std::floor(y)]; }
	inline void AddPaintBlobAtPos(std::shared_ptr<PaintBlob> newObject, float x, float y) { m_PaintBlobs[std::floor(x)][std::floor(y)] = newObject; }

	inline std::shared_ptr<Belt> GetBeltAtPos(float x, float y) { return m_Belts[std::floor(x)][std::floor(y)]; }
	inline void AddBeltAtPos(std::shared_ptr <Belt> newBelt, float x, float y) { m_Belts[std::floor(x)][std::floor(y)] = newBelt; }

	inline std::shared_ptr<GameObject> GetGameObjectAtPos(float x, float y) { return m_GameObjects[std::floor(x)][std::floor(y)]; }
	inline void AddGameObjectAtPos(std::shared_ptr <GameObject> newObject, float x, float y) { m_GameObjects[std::floor(x)][std::floor(y)] = newObject; }

};