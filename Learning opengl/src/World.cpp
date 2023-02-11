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
    std::vector<Belt>& belts = GetBeltsAtPos(x, y);
    for (int i = 0; i < belts.size(); i++)
    {
        return belts[i].GetDirection();
    }
    return -1;
}

bool World::AddGameObject(GameObject& newObject)
{
    std::vector<GameObject>& gameObjects = GetGameObjectsAtPos(newObject.GetPos().x, newObject.GetPos().y);
    if (gameObjects.size() == 0) // if there is nothing on that tile
    {
        m_GameObjects[newObject.GetPos().x][newObject.GetPos().y].push_back(newObject);
        return true;
    }

    return false;
}

bool World::AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size)
{
    std::vector<GameObject>& gameObjects = GetGameObjectsAtPos(pos.x, pos.y);
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i].GetTile()->GetType() == 0) // there is a blob there already
        {
            return false;
        }
    }

    // there is no blob there
    WorldTile tile(TileTypePaintBlob, BlobColor);
    GameObject paintBlob(tile, pos+.5f, size * m_BlockSize);

    if (GetBeltsAtPos(paintBlob.GetPos().x, paintBlob.GetPos().y).size() != 0)
    {
        if (GetBeltsAtPos(paintBlob.GetPos().x, paintBlob.GetPos().y)[0].GetAllowNewItem())
        {
            if (GetBeltsAtPos(paintBlob.GetPos().x, paintBlob.GetPos().y)[0].GetDirection() == DirectionUp || GetBeltsAtPos(paintBlob.GetPos().x, paintBlob.GetPos().y)[0].GetDirection() == DirectionDown)
                paintBlob.SetPos(paintBlob.GetPos() + Vec3{ 0.0f, -.5f, 0.0f });
            else
                paintBlob.SetPos(paintBlob.GetPos() + Vec3{ -.5f, 0.0f, 0.0f });
            GetBeltsAtPos(paintBlob.GetPos().x, paintBlob.GetPos().y)[0].AddObject(paintBlob); // dont add to gameObj list if its in a belt
        }
    }
    else AddGameObjectAtPos(paintBlob, paintBlob.GetPos().x, paintBlob.GetPos().y);

    return true;
    
    
}

bool World::AddBelt(BeltType beltColor, Vec3 pos, Direction direction)
{
    Belt belt(WorldTile(TileTypeStraightBelt), pos, m_BlockSize, direction, beltColor, m_Belts);

    if (direction == DirectionUp)
    {
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        if (leftBelt == DirectionRight)
        {
            belt.GetTile()->SetType(TileTypeTurnBelt);
        }
        else if (rightBelt == DirectionLeft)
        {
            belt.GetTile()->SetType(TileTypeTurnBeltBackwards);
        }
    }
    if (direction == DirectionRight)
    {
        int upBelt = GetBeltDirectionAt(pos.x, pos.y + 1);
        int downBelt = GetBeltDirectionAt(pos.x, pos.y - 1);
        if (upBelt == DirectionDown)
        {
            belt.GetTile()->SetType(TileTypeTurnBeltBackwards);
            belt.SetDirection(DirectionRight);
        }
        else if (downBelt == DirectionUp)
        {
            belt.GetTile()->SetType(TileTypeTurnBelt);
            belt.SetDirection(DirectionRight);
        }
    }
    if (direction == DirectionDown)
    {
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        if (rightBelt == DirectionLeft)
        {
            belt.GetTile()->SetType(TileTypeTurnBelt);
            belt.SetDirection(DirectionDown);
        }
        else if (leftBelt == DirectionRight)
        {
            belt.GetTile()->SetType(TileTypeTurnBeltBackwards);
        }
    }
    if (direction == DirectionLeft)
    {
        int downBelt = GetBeltDirectionAt(pos.x, pos.y - 1);

        int upBelt = GetBeltDirectionAt(pos.x, pos.y + 1);
        if (downBelt == DirectionUp)
        {
            belt.GetTile()->SetType(TileTypeTurnBeltBackwards);
            belt.SetDirection(DirectionLeft);
        }
        else if (upBelt == DirectionDown)
        {
            belt.GetTile()->SetType(TileTypeTurnBelt);
        }
    }

    
    if (!AddBeltToWorld(belt))
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

    // update all the belts
    for (auto& row : m_Belts)
    {
        int x = row.first;
        for (auto& col : row.second)
        {
            int y = col.first;
            std::vector<Belt>& belts = col.second;
            for (int i = 0; i < belts.size(); i++)
            {
                if (belts[i].GetNextBelt() == nullptr)
                    belts[i].Update(); // this will call all the belts in the chain to update
            }
        }
    }

    // update all the game objects
    for (auto& row : m_GameObjects)
    {
        int x = row.first;
        for (auto& col : row.second)
        {
            int y = col.first;
            std::vector<GameObject>& gameObjects = col.second;
            for (int i = 0; i < gameObjects.size(); i++)
            {
                gameObjects[i].Update();
            }

        }
    }

    input->Reset();
}


bool World::AddBeltToWorld(Belt& belt)
{
    std::vector<Belt>& belts = GetBeltsAtPos(belt.GetPos().x, belt.GetPos().y);
    std::vector<GameObject>& gameObjects = GetGameObjectsAtPos(belt.GetPos().x, belt.GetPos().y);

    if (belts.size() == 0) // there is no belt here already
    {
        AddBeltAtPos(belt, belt.GetPos().x, belt.GetPos().y);
        return true;
    }

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i].GetTile()->GetType() > TileTypeRedArrow)
        {
            return false; // this means there is something on this space other than a belt
        }
    }

    {
        // remove what ever is there and add belt if it is not the same
        if ( !(belt == m_Belts[std::floor(belt.GetPos().x)][std::floor(belt.GetPos().y)][0]) )
        {
            m_Belts[std::floor(belt.GetPos().x)][std::floor(belt.GetPos().y)].clear();
            AddBeltAtPos(belt, belt.GetPos().x, belt.GetPos().y);
            return true;
        }
    }

    return false;
}

void World::DeleteAllAtPos(Vec3 pos)
{
    m_GameObjects[std::floor(pos.x)][std::floor(pos.y)].clear();
    m_Belts[std::floor(pos.x)][std::floor(pos.y)].clear();
}
