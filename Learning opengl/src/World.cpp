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
    std::vector<GameObject> gameObjects = m_GameObjects[x][y];
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i].GetTile().GetType() == TileTypeStraightBelt || gameObjects[i].GetTile().GetType() == TileTypeTurnBelt || gameObjects[i].GetTile().GetType() == TileTypeTurnBeltBackwards)
            return gameObjects[i].GetDirection();
    }
    return -1;
}

void World::UpdateGameObjectPositionsAtPos(int x, int y)
{
    std::vector<GameObject>& gameObjects = m_GameObjects[x][y];
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i].GetPos().x != x || gameObjects[i].GetPos().y != y)
        {
            ReAddGameObject(gameObjects[i]);
            gameObjects.erase(gameObjects.begin() + i);
        }
    }
}

bool World::ReAddGameObject(GameObject& object)
{
    m_GameObjects[object.GetPos().x][object.GetPos().y].push_back(object);
    return true;
}

bool World::AddGameObject(GameObject newObject)
{
    std::vector<GameObject> worldTiles = m_GameObjects[newObject.GetPos().x][newObject.GetPos().y];
    if (worldTiles.size() == 0) // if there is nothing on that tile
    {
        m_GameObjects[newObject.GetPos().x][newObject.GetPos().y].push_back(newObject);
        return true;
    }

    return false;
}

bool World::AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size)
{
    std::vector<GameObject> gameObjects = m_GameObjects[pos.x][pos.y];
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i].GetTile().GetType() == 0) // there is a blob there already
        {
            return false;
        }
    }
    // there is no blob there
    WorldTile tile(TileTypePaintBlob, BlobColor);
    GameObject paintBlob(tile, pos, size * m_BlockSize);

    m_GameObjects[paintBlob.GetPos().x][paintBlob.GetPos().y].push_back(paintBlob);
    return true;
    
    
}

bool World::AddBelt(BeltType beltColor, Vec3 pos, Direction direction)
{
    WorldTile beltBack(TileTypeStraightBelt);
    // WorldTile arrow(beltColor, pos, m_BlockSize);

    Belt belt(beltBack, pos, m_BlockSize, direction, beltColor);

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
            belt.SetDirection(DirectionRight);
        }
        else if (downBelt == DirectionUp)
        {
            beltBack.SetType(TileTypeTurnBelt);
            belt.SetDirection(DirectionRight);
        }
    }
    if (direction == DirectionDown)
    {
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        if (rightBelt == DirectionLeft)
        {
            beltBack.SetType(TileTypeTurnBelt);
            belt.SetDirection(DirectionDown);
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
            belt.SetDirection(DirectionLeft);
        }
        else if (upBelt == DirectionDown)
        {
            beltBack.SetType(TileTypeTurnBelt);
        }
    }
    
    if (!AddBelt(belt))
        return false;

    return true;
}


void World::OnUpdate(Input* input)
{
    // do inputs
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

    float mousePosX = ((((input->GetMousePosX() / (WIDTH / 2)) * zoomedWidth) - zoomedWidth) - m_Position.x) / m_BlockSize;
    float mousePosY = -1 * ((((input->GetMousePosY() / (HEIGHT / 2)) * zoomedHeight) - zoomedHeight) + m_Position.y) / m_BlockSize;

    if (mousePosX < 0) mousePosX = floor(mousePosX);
    else mousePosX = floor(mousePosX);
    if (mousePosY < 0) mousePosY = floor(mousePosY);
    else mousePosY = floor(mousePosY);

    if (input->GetLeftMouseDown())
    {
        switch (input->GetLastNumPressed()) 
        {
            case(1): AddBelt(BeltTypeYellow, { mousePosX, mousePosY, 1 }, (Direction)input->GetDirection()); break;
            case(2): AddBelt(BeltTypeOrange, { mousePosX, mousePosY, 1 }, (Direction)input->GetDirection()); break;
            case(3): AddBelt(BeltTypeRed,    { mousePosX, mousePosY, 1 }, (Direction)input->GetDirection()); break;
            case(4): AddPaintBlob(Vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .1); break;
            case(5): AddPaintBlob(Vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(6): AddPaintBlob(Vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .3); break;
            default: std::cout << "No tile for that number yet" << std::endl;
        }
    }
    if (input->GetRightMouseDown())
    {
        DeleteAllAtPos(Vec3{ mousePosX, mousePosY, 1 });
    }

    input->Reset();
}


bool World::AddBelt(Belt belt)
{
    std::vector<Belt> belts = m_Belts[belt.GetPos().x][belt.GetPos().y];
    std::vector<GameObject> gameObjects = m_GameObjects[belt.GetPos().x][belt.GetPos().y];

    if (belts.size() == 0) // there is no belt here already
    {
        m_Belts[belt.GetPos().x][belt.GetPos().y].push_back(belt);
        return true;
    }

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i].GetTile().GetType() > TileTypeRedArrow)
        {
            return false; // this means there is something on this space other than a belt
        }
    }

    {
        // remove what ever is there and add belt
        m_Belts[belt.GetPos().x][belt.GetPos().y].clear();
        m_Belts[belt.GetPos().x][belt.GetPos().y].push_back(belt);
        return true;
    }

    return false;
}

void World::DeleteAllAtPos(Vec3 pos)
{
    m_GameObjects[pos.x][pos.y].clear();
    m_Belts[pos.x][pos.y].clear();
}
