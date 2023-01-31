#include "World.h"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>


World::World(GLFWwindow* window)
    :m_Window(window), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Speed(glm::vec3(0.0f, 0.0f, 0.0f)), m_ZoomAmount(.25), m_ZoomChange(0)
{
}

World::World(GLFWwindow* window, glm::vec3 position)
    :m_Window(window), m_Position(position), m_Speed(glm::vec3(0.0f, 0.0f, 0.0f)), m_ZoomAmount(.25), m_ZoomChange(0)
{
}

World::World(GLFWwindow* window, glm::vec3 position, glm::vec3 speed)
    :m_Window(window), m_Position(position), m_Speed(speed), m_ZoomAmount(.25), m_ZoomChange(0)
{
}

World::~World()
{
}

void World::checkKeyPresses()
{
    int action;
    action = glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT);
    float MOVE_SPEED = ORIGINAL_MOVE_SPEED;
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        MOVE_SPEED *= 2;
    }
    action = glfwGetKey(m_Window, GLFW_KEY_W);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        m_Speed += glm::vec3(0.0f, -1*MOVE_SPEED, 0.0f);
    }
    action = glfwGetKey(m_Window, GLFW_KEY_S);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        m_Speed += glm::vec3(0.0f, MOVE_SPEED, 0.0f);
    }
    action = glfwGetKey(m_Window, GLFW_KEY_A);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        
        m_Speed += glm::vec3(MOVE_SPEED, 0.0f, 0.0f);
    }
    action = glfwGetKey(m_Window, GLFW_KEY_D);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        
        m_Speed += glm::vec3(-1*MOVE_SPEED, 0.0f, 0.0f);
    }
    action = glfwGetKey(m_Window, GLFW_KEY_T);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        m_ZoomChange += ZOOM_CHANGE_BY;
    }
    action = glfwGetKey(m_Window, GLFW_KEY_Y);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        m_ZoomChange -= ZOOM_CHANGE_BY;
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
    m_Speed = glm::vec3(0.0f);
    m_ZoomChange = 0.0f;
}
