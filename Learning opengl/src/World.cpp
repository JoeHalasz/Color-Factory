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

int World::GetBeltDirectionAt(int x, int y)
{
    std::vector<WorldTile> tiles = m_WorldTiles[x][y];
    for (int i = 0; i < tiles.size(); i++)
    {
        if (tiles[i].GetType() == TileTypeStraightBelt || tiles[i].GetType() == TileTypeTurnBelt || tiles[i].GetType() == TileTypeTurnBeltBackwards)
            return tiles[i].GetDirection();
    }
    return -1;
}

bool World::AddPaintBlob(Vec4 BlobColor, glm::vec3 pos, float size)
{
    WorldTile paintBlob(TileTypePaintBlob, pos, size * m_BlockSize, (Direction)0);
    paintBlob.SetColor(BlobColor); // red
    m_WorldTiles[paintBlob.GetPos().x][paintBlob.GetPos().y].push_back(paintBlob);
    return true;
}

bool World::AddBelt(TileType beltColor, glm::vec3 pos, Direction direction)
{

    WorldTile beltBack(TileTypeStraightBelt, pos, m_BlockSize, direction);
    WorldTile arrow(beltColor, pos, m_BlockSize, direction);

    if (direction == DirectionUp)
    {
        
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        if (leftBelt == DirectionRight)
        {
            beltBack.SetType(TileTypeTurnBelt);
        }
        else if (rightBelt == DirectionLeft)
        {
            beltBack.SetType(TileTypeTurnBeltBackwards);
        }
    }
    if (direction == DirectionRight)
    {
        int upBelt = GetBeltDirectionAt(pos.x, pos.y + 1);
        int downBelt = GetBeltDirectionAt(pos.x, pos.y - 1);
        if (upBelt == DirectionDown)
        {
            beltBack.SetType(TileTypeTurnBeltBackwards);
            beltBack.SetDirection(DirectionRight);
        }
        else if (downBelt == DirectionUp)
        {
            beltBack.SetType(TileTypeTurnBelt);
            beltBack.SetDirection(DirectionRight);
        }
    }
    if (direction == DirectionDown)
    {
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        if (rightBelt == DirectionLeft)
        {
            beltBack.SetType(TileTypeTurnBelt);
            beltBack.SetDirection(DirectionDown);
        }
        else if (leftBelt == DirectionRight)
        {
            beltBack.SetType(TileTypeTurnBeltBackwards);
        }
    }
    if (direction == DirectionLeft)
    {
        int downBelt = GetBeltDirectionAt(pos.x, pos.y - 1);

        int upBelt = GetBeltDirectionAt(pos.x, pos.y + 1);
        if (downBelt == DirectionUp)
        {
            beltBack.SetType(TileTypeTurnBeltBackwards);
            beltBack.SetDirection(DirectionLeft);
        }
        else if (upBelt == DirectionDown)
        {
            beltBack.SetType(TileTypeTurnBelt);
        }
    }
    
    if (!AddWorldTileBelt(beltBack))
        return false;
    if (!AddWorldTileArrow(arrow))
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
            case(4): AddPaintBlob(Vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, glm::vec3(mousePosX, mousePosY, 1), .1); break;
            case(5): AddPaintBlob(Vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, glm::vec3(mousePosX, mousePosY, 1), .2); break;
            case(6): AddPaintBlob(Vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec3(mousePosX, mousePosY, 1), .3); break;
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

bool World::AddWorldTileArrow(WorldTile tile)
{
    m_WorldTiles[tile.GetPos().x][tile.GetPos().y].push_back(tile);
    return true;
}

bool World::AddWorldTileBelt(WorldTile tile)
{
    std::vector<WorldTile> worldTiles = m_WorldTiles[tile.GetPos().x][tile.GetPos().y];

    if (worldTiles.size() == 0) // there is no belt here already
    {
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].push_back(tile);
        return true;
    }

    for (int i = 0; i < worldTiles.size(); i++)
    {
        if (worldTiles[i].GetType() > TileTypeRedArrow)
        {
            return false; // this means there is something on this space other than a belt
        }
    }

    {
        // remove what ever is there and add belt
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].clear();
        m_WorldTiles[tile.GetPos().x][tile.GetPos().y].push_back(tile);
        return true;
    }

    return false;
}

void World::DeleteAllInTile(glm::vec3 pos)
{
    m_WorldTiles[pos.x][pos.y].clear();
}
