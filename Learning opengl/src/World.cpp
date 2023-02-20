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
    std::shared_ptr<Belt> belt = GetBeltAtPos(x, y);
    if (belt != NULL)
    {
        return belt->GetDirection();
    }
    return -1;
}

bool World::AddPaintBlob(std::shared_ptr<PaintBlob> newObject)
{
    AddPaintBlobAtPos(newObject, newObject->GetPos().x, newObject->GetPos().y);

    return true;
}

bool World::AddPaintBlob(Vec4 BlobColor, Vec3 pos, float size)
{
    std::shared_ptr<PaintBlob> checkPaintBlob = GetPaintBlobAtPos(pos.x, pos.y);
    if (checkPaintBlob != NULL)
    {
        return false;
    }

    // there is no blob there
    std::shared_ptr<PaintBlob> paintBlob(new PaintBlob(pos + .5f, size * m_BlockSize, BlobColor));
    if (GetBeltAtPos(pos.x, pos.y) != NULL)
    {
        if (GetBeltAtPos(pos.x, pos.y)->AllowNewItem(true))
        {
            // set steps to half
            GetBeltAtPos(paintBlob->GetPos().x, paintBlob->GetPos().y)->AddObject(paintBlob, true); // dont add to gameObj list if its in a belt
        }
    }
    else AddPaintBlobAtPos(paintBlob, paintBlob->GetPos().x, paintBlob->GetPos().y);

    return true;
}

bool World::NothingAtPos(Vec3 pos)
{
    if (GetPaintBlobAtPos(pos.x, pos.y) != NULL)
    {
        return false;
    }
    if (GetBeltAtPos(pos.x, pos.y) != NULL)
    {
        return false;
    }
    if (GetGameObjectAtPos(pos.x, pos.y) != NULL)
    {
        return false;
    }
    return true;
}


bool World::AddBelt(BeltType beltColor, Vec3 pos, Direction direction)
{
    if (!BeltCanBeMade(pos, beltColor, direction))
        return false;

    std::shared_ptr<Belt> belt(new Belt(WorldTile(TileTypeStraightBelt), pos, m_BlockSize, direction, beltColor));

    if (direction == DirectionUp)
    {
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        if (leftBelt == DirectionRight)
        {
            belt->GetTile()->SetType(TileTypeTurnBelt);
        }
        else if (rightBelt == DirectionLeft)
        {
            belt->GetTile()->SetType(TileTypeTurnBeltBackwards);
        }
    }
    if (direction == DirectionRight)
    {
        int upBelt = GetBeltDirectionAt(pos.x, pos.y + 1);
        int downBelt = GetBeltDirectionAt(pos.x, pos.y - 1);
        if (upBelt == DirectionDown)
        {
            belt->GetTile()->SetType(TileTypeTurnBeltBackwards);
            belt->SetDirection(DirectionRight);
        }
        else if (downBelt == DirectionUp)
        {
            belt->GetTile()->SetType(TileTypeTurnBelt);
            belt->SetDirection(DirectionRight);
        }
    }
    if (direction == DirectionDown)
    {
        int rightBelt = GetBeltDirectionAt(pos.x + 1, pos.y);
        int leftBelt = GetBeltDirectionAt(pos.x - 1, pos.y);
        if (rightBelt == DirectionLeft)
        {
            belt->GetTile()->SetType(TileTypeTurnBelt);
            belt->SetDirection(DirectionDown);
        }
        else if (leftBelt == DirectionRight)
        {
            belt->GetTile()->SetType(TileTypeTurnBeltBackwards);
        }
    }
    if (direction == DirectionLeft)
    {
        int downBelt = GetBeltDirectionAt(pos.x, pos.y - 1);

        int upBelt = GetBeltDirectionAt(pos.x, pos.y + 1);
        if (downBelt == DirectionUp)
        {
            belt->GetTile()->SetType(TileTypeTurnBeltBackwards);
            belt->SetDirection(DirectionLeft);
        }
        else if (upBelt == DirectionDown)
        {
            belt->GetTile()->SetType(TileTypeTurnBelt);
        }
    }
    // remove what ever is there and add belt if it is not the same
    if (m_Belts[std::floor(belt->GetPos().x)][std::floor(belt->GetPos().y)] != NULL)
    {
        m_Belts[std::floor(belt->GetPos().x)][std::floor(belt->GetPos().y)] = NULL;
    }
    if (m_PaintBlobs[std::floor(belt->GetPos().x)][std::floor(belt->GetPos().y)] != NULL)
    {
        m_PaintBlobs[std::floor(belt->GetPos().x)][std::floor(belt->GetPos().y)] = NULL;// this will only be blobs because of check at beginning of func
    }
    AddBeltAtPos(belt, belt->GetPos().x, belt->GetPos().y);
    belt->SetUpNextAndLastBelt(m_Belts);

    return true;
}


bool World::AddPaintBlobCombiner(Vec3 pos, Direction direction)
{
    if (!NothingAtPos(pos))
        return false;
    std::shared_ptr<GameObject> paintBlobCombiner(new PaintBlobCombiner(pos, GetBlockSize(), direction, true));
    AddGameObjectAtPos(paintBlobCombiner, pos.x, pos.y);
    Vec3 otherPos;
    if (direction == DirectionUp)
        otherPos = pos + Vec3{ 1,0,0 };
    else if (direction == DirectionDown)
        otherPos = pos + Vec3{ -1,0,0 };
    else if (direction == DirectionLeft)
        otherPos = pos + Vec3{ 0, -1,0 };
    else /*if (direction == DirectionRight)*/
        otherPos = pos + Vec3{ 0, 1,0 };

    std::shared_ptr<GameObject> paintBlobCombiner2(new PaintBlobCombiner(otherPos, GetBlockSize(), direction, false, TileTypePaintBlobContainer2));
    paintBlobCombiner2->SetParentObject(paintBlobCombiner);
    paintBlobCombiner->AddOtherPart(paintBlobCombiner2);
    AddGameObjectAtPos(paintBlobCombiner2, otherPos.x, otherPos.y);
    
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
            case(4): AddPaintBlob(Vec4{ 1.0f, 0.0f, 0.0f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(5): AddPaintBlob(Vec4{ 0.0f, 1.0f, 0.0f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(6): AddPaintBlob(Vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(7): AddPaintBlobCombiner(Vec3{ mousePosX, mousePosY,1 }, (Direction)input->GetDirection()); break;
            default: std::cout << "No tile for that number yet" << std::endl;
        }
    }
    if (input->GetRightMouseDown())
    {
        DeleteAllAtPos(Vec3{ mousePosX, mousePosY, 1 });
    }

    std::vector<std::shared_ptr<Belt>> heads;

    // find all the belts that have a head and reset UpdatedThisFrame
    for (auto& row : m_Belts)
    {
        int x = row.first;
        for (auto& col : row.second)
        {
            int y = col.first;
            std::shared_ptr<Belt> belt = col.second;
            if (belt != NULL)
            {
                if (belt->GetNextBelt() == NULL)
                    heads.push_back(belt);
                belt->UpdatedThisFrame = false;
            }
        }
    }

    // update the heads. This will update every belt in that chain
    for (int i = 0; i < heads.size(); i++)
    {
        if (heads[i]->GetNextBelt() == NULL)
            heads[i]->Update(); // this will call all the belts in the chain to update
    }


    // update all the belts that dont have a head
    for (auto& row : m_Belts)
    {
        int x = row.first;
        for (auto& col : row.second)
        {
            int y = col.first;
            std::shared_ptr<Belt> belt = col.second;
            if (belt != NULL)
                if (!belt->UpdatedThisFrame)
                    belt->Update(); // this will call all the belts in the chain to update
            
        }
    }

    // update all the game objects
    for (auto& row : m_PaintBlobs)
    {
        int x = row.first;
        for (auto& col : row.second)
        {
            int y = col.first;
            std::shared_ptr<PaintBlob> paintBlobs = col.second;
            if (paintBlobs != NULL)
            {
                // paintBlob.Update();
            }

        }
    }

    input->Reset();
}


bool World::BeltCanBeMade(Vec3 pos, BeltType beltColor, Direction direction)
{

    std::shared_ptr<Belt> belt = GetBeltAtPos(pos.x, pos.y);
    std::shared_ptr<PaintBlob> paintBlob = GetPaintBlobAtPos(pos.x, pos.y);

    if (belt != NULL) // there is no belt here already
    {
        return true;
    }

    if (paintBlob != NULL)
        return false; // this means there is something on this space other than a belt

    std::shared_ptr<Belt> other = GetBeltAtPos(pos.x, pos.y);
    if (other != NULL)
        if (other->GetPos() == pos && other->GetBeltType() == beltColor && other->GetDirection() == direction)
            return false;

    return true;
}

void World::DeleteAllAtPos(Vec3 pos)
{
    m_PaintBlobs[std::floor(pos.x)][std::floor(pos.y)] = NULL;
    m_Belts[std::floor(pos.x)][std::floor(pos.y)] = NULL;
    if (GetGameObjectAtPos(pos.x, pos.y) != NULL)
    {
        std::shared_ptr<GameObject> objectHere = GetGameObjectAtPos(pos.x, pos.y);
        if (!objectHere->GetIsBasePart())
        {
            for (int j = 0; j < objectHere->GetOtherParts().size(); j++)
            {
                m_GameObjects[objectHere->GetOtherParts()[j]->GetPos().x][objectHere->GetOtherParts()[j]->GetPos().y] = NULL;
            }
        }
        else {
            m_GameObjects[objectHere->GetParentObject()->GetPos().x][objectHere->GetParentObject()->GetPos().y] = NULL;
        }
    }
    m_GameObjects[std::floor(pos.x)][std::floor(pos.y)] = NULL;
}
