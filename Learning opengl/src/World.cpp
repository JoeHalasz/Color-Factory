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


bool World::AddBelt(TileType beltColor, glm::vec3 pos)
{
    WorldTile beltBack(TileTypeSingleBelt, pos, m_BlockSize);
    WorldTile arrow(beltColor, pos, m_BlockSize);
    if (!AddWorldTile(beltBack))
        return false;
    if (!AddWorldTile(arrow))
        return false;
    return true;
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
    
    
    float zoomedWidth = (WIDTH / 2) + (m_ZoomAmount * (WIDTH / 20));
    float zoomedHeight = (HEIGHT / 2) + (m_ZoomAmount * (HEIGHT / 20));
    
    
    double mousePosX = ((((input->GetMousePosX() / (WIDTH / 2)) * zoomedWidth) - zoomedWidth) - m_Position.x) / m_BlockSize;
    double mousePosY = -1 * ((((input->GetMousePosY() / (HEIGHT / 2)) * zoomedHeight) - zoomedHeight) + m_Position.y) / m_BlockSize;

    if (mousePosX < 0) mousePosX = floor(mousePosX);
    else mousePosX = floor(mousePosX);
    if (mousePosY < 0) mousePosY = floor(mousePosY);
    else mousePosY = floor(mousePosY);

    if (input->GetMouseDown())
    {
        switch (input->GetLastNumPressed()) {
        case(1): AddBelt(TileTypeYellowArrow, glm::vec3(mousePosX * m_BlockSize, mousePosY * m_BlockSize, 1)); break;
        case(2): AddBelt(TileTypeOrangeArrow, glm::vec3(mousePosX * m_BlockSize, mousePosY * m_BlockSize, 1)); break;
        case(3): AddBelt(TileTypeRedArrow, glm::vec3(mousePosX * m_BlockSize, mousePosY * m_BlockSize, 1)); break;
        default: std::cout << "No tile for that number yet" << std::endl;
        }
    }


    input->Reset();
}

bool World::AddWorldTile(WorldTile tile)
{
    m_WorldTiles.push_back(tile);
    return true;
}
