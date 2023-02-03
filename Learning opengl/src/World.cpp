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

World::~World()
{
}


void World::OnUpdate(Input* input)
{
    input->CheckInputPresses();
    
    m_ZoomAmount -= input->GetChangeZoom();
    if (m_ZoomAmount >= ZOOM_MAX)
        m_ZoomAmount = ZOOM_MAX;
    if (m_ZoomAmount <= ZOOM_MIN) 
        m_ZoomAmount = ZOOM_MIN;
    
    m_Position += input->GetSpeed() + (input->GetSpeed() * (float)m_ZoomAmount);

    m_Rotation += input->GetRotationChange();
    int WIDTH, HEIGHT;
    glfwGetWindowSize(m_Window, &WIDTH, &HEIGHT);

    // fix this 
    //m_MousePosX = (int)(input->GetMousePosX() - ((WIDTH / 2.0f) ))/50;
    //m_MousePosY = -1*(int)(input->GetMousePosY() - ((HEIGHT / 2.0f) ))/50;


    input->Reset();
}
