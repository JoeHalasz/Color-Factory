#pragma once

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"

#include "Input.h"

#include "GameObject.h"
#include "Belt.h"

class World
{
private:
	GLFWwindow* m_Window;
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_ZoomAmount = 0;
	int m_BlockSize = 75;

	float m_Rotation = 0;
	float ZOOM_MAX = 25;
	float ZOOM_MIN = 0;

	int m_MousePosX = 0;
	int m_MousePosY = 0;

	std::unordered_map<int, std::unordered_map<int, std::vector<GameObject>>> m_GameObjects;
	std::unordered_map<int, std::unordered_map<int, std::vector<Belt>>> m_Belts;

	bool AddBelt(BeltType beltColor, Vec3 pos, Direction direction);
	bool AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size);

public:
	bool IS3D = false; // Only works if there is only one texture
	
	World(GLFWwindow* window);
	World(GLFWwindow* window, glm::vec3 position);
	~World();
	void OnUpdate(Input* input);
	bool AddGameObject(GameObject& newObject);
	bool AddBeltToWorld(Belt& belt);
	void DeleteAllAtPos(Vec3 pos);
	int GetBeltDirectionAt(int x, int y);

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline int GetZoomAmount() const { return (int)m_ZoomAmount;  }
	inline float GetRotation() const { return m_Rotation;  }
	inline int GetBlockSize() { return m_BlockSize; }
	inline int GetMousePosX() { return m_MousePosX; }
	inline int GetMousePosY() { return m_MousePosY; }

	inline std::vector<GameObject>& GetGameObjectsAtPos(float x, float y) { return m_GameObjects[std::floor(x)][std::floor(y)]; }
	inline GameObject AddGameObjectAtPos(GameObject& newObject, float x, float y) { m_GameObjects[std::floor(x)][std::floor(y)].emplace_back(newObject); return newObject; } // will return the object
	
	inline std::vector<Belt>& GetBeltsAtPos(float x, float y) { return m_Belts[std::floor(x)][std::floor(y)]; }
	inline void AddBeltAtPos(Belt& newBelt, float x, float y) { return m_Belts[std::floor(x)][std::floor(y)].emplace_back(newBelt); }

};