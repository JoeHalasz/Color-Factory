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


bool World::AddBelt(TileType beltColor, glm::vec3 pos, Direction direction)
{
    WorldTile beltBack(TileTypeStraightBelt, pos, m_BlockSize, direction);
    WorldTile arrow(beltColor, pos, m_BlockSize, direction);
    if (!AddWorldTileBelt(beltBack))
        return false;
    if (!AddWorldTileBelt(arrow))
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
    
    m_Position += input->GetSpeed() + (input->GetSpeed() * ((float)m_ZoomAmount/10));
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

    if (input->GetLeftMouseDown())
    {
        switch (input->GetLastNumPressed()) 
        {
            case(1): AddBelt(TileTypeYellowArrow, glm::vec3(mousePosX, mousePosY, 1), (Direction)input->GetDirection()); break;
            case(2): AddBelt(TileTypeOrangeArrow, glm::vec3(mousePosX, mousePosY, 1), (Direction)input->GetDirection()); break;
            case(3): AddBelt(TileTypeRedArrow, glm::vec3(mousePosX, mousePosY, 1), (Direction)input->GetDirection()); break;
            default: std::cout << "No tile for that number yet" << std::endl;
        }
    }
    if (input->GetRightMouseDown())
    {
        DeleteAllInTile(glm::vec3(mousePosX, mousePosY, 1));
    }

    input->Reset();
}

bool World::AddWorldTile(WorldTile tile)
{
    std::vector<WorldTile> worldTiles = m_WorldTiles[tile.GetPos().x][tile.GetPos().y];
    if (worldTiles.size() == 0) // if there is nothing on that tile
    {
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].push_back(tile);
        return true;
    }
        
    return false;
}

bool World::AddWorldTileBelt(WorldTile tile)
{
    std::vector<WorldTile> worldTiles = m_WorldTiles[tile.GetPos().x][tile.GetPos().y];

    if (worldTiles.size() < 2) // there is no belt here already
    {
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].push_back(tile);
        return true;
    }

    for (int i = 0; i < worldTiles.size(); i++)
    {
        if (worldTiles[i].GetType() != TileTypeStraightBelt && worldTiles[i].GetType() != TileTypeTurnBelt && 
            worldTiles[i].GetType() != TileTypeYellowArrow && worldTiles[i].GetType() != TileTypeOrangeArrow && 
            worldTiles[i].GetType() != TileTypeRedArrow)
        {
            return false; // this means there is something on this space other than a belt
        }
    }
    if (worldTiles.size() >= 2) // there is no belt here already
    {
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].clear(); // remove the belt thats there already
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].push_back(tile);
        return true;
    }

    return false;
}

void World::DeleteAllInTile(glm::vec3 pos)
{
    m_WorldTiles[pos.x][pos.y].clear();
}
