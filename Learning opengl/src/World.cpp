#include "World.h"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>

World::World(GLFWwindow* window)
    :m_Window(window)
{
    if (IS3D)
    {
        m_Position.z = -350;
    }
}

World::World(GLFWwindow* window, glm::vec3 position)
    :m_Window(window), m_Position(position)
{
    if (IS3D)
    {
        m_Position.z = -350;
    }
}

World::World(GLFWwindow* window, glm::vec3 position, glm::vec3 speed)
    :m_Window(window), m_Position(position), m_Speed(speed)
{
    if (IS3D)
    {
        m_Position.z = -350;
    }
}

World::~World()
{
}

bool World::KeyPressed(int key)
{
    int action = glfwGetKey(m_Window, key);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) return true;
    return false;
}

void World::checkKeyPresses()
{
    float MOVE_SPEED = ORIGINAL_MOVE_SPEED;
    int action;

    if (KeyPressed(GLFW_KEY_LEFT_SHIFT)) MOVE_SPEED *= 2;

    if (KeyPressed(GLFW_KEY_W)) m_Speed += glm::vec3(0.0f, -1*MOVE_SPEED, 0.0f);
    if (KeyPressed(GLFW_KEY_S)) m_Speed += glm::vec3(0.0f, MOVE_SPEED, 0.0f);
    if (KeyPressed(GLFW_KEY_A)) m_Speed += glm::vec3(MOVE_SPEED, 0.0f, 0.0f);
    if (KeyPressed(GLFW_KEY_D)) m_Speed += glm::vec3(-1*MOVE_SPEED, 0.0f, 0.0f);

    if (KeyPressed(GLFW_KEY_T)) m_ZoomChange += ZOOM_CHANGE_BY;
    if (KeyPressed(GLFW_KEY_Y)) m_ZoomChange -= ZOOM_CHANGE_BY;
    if (KeyPressed(GLFW_KEY_SPACE)) m_Position = glm::vec3(0, 0, 0);
    if (IS3D) {
        if (KeyPressed(GLFW_KEY_E)) m_Speed += glm::vec3(0.0f, 0.0f, -1 * MOVE_SPEED);
        if (KeyPressed(GLFW_KEY_Q)) m_Speed += glm::vec3(0.0f, 0.0f, MOVE_SPEED);
        if (KeyPressed(GLFW_KEY_G)) m_RotationChange += ROTATION_SPEED;
        if (KeyPressed(GLFW_KEY_H)) m_RotationChange -= ROTATION_SPEED;
    }
    
    
}

void World::OnUpdate()
{
    checkKeyPresses();
    
    m_ZoomAmount += m_ZoomChange;
    if (m_ZoomAmount >= ZOOM_MAX)
        m_ZoomAmount = ZOOM_MAX;
    if (m_ZoomAmount <= ZOOM_MIN) {
        m_ZoomAmount = ZOOM_MIN;
    }
    m_Position += m_Speed + (m_Speed * m_ZoomAmount);
    m_Rotation += m_RotationChange;
    m_RotationChange = 0;
    m_Speed = glm::vec3(0.0f);
    m_ZoomChange = 0.0f;
}
