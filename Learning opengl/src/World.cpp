#include "World.h"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>

World::World(GLFWwindow* window)
    :m_Window(window)
{
    if (IS3D)
        m_Position.z = -350;
}

World::World(GLFWwindow* window, glm::vec3 position)
    :m_Window(window), m_Position(position)
{
    if (IS3D)
        m_Position.z = -350;
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
    belt->SetUpNextAndLastObject(m_Belts, m_GameObjects);

    return true;
}


bool World::AddPaintBlobCombiner(Vec3 pos, Direction direction, int numInputs)
{
    if (!NothingAtPos(pos))
        return false;
    if (!NothingAtPos(Vec3{ pos.x + 1, pos.y, pos.z }))
        return false;

    std::shared_ptr<GameObject> paintBlobCombiner(new PaintBlobCombiner(pos, GetBlockSize(), numInputs, direction, true));
    AddGameObjectAtPos(paintBlobCombiner, pos.x, pos.y);
    Vec3 otherPos1;
    Vec3 otherPos2;
    if (direction == DirectionUp) {
        otherPos1 = pos + Vec3{ 1,0,0 };
        otherPos2 = pos + Vec3{ -1,0,0 };
    }
    else if (direction == DirectionDown) {
        otherPos1 = pos + Vec3{ -1,0,0 };
        otherPos2 = pos + Vec3{ 1,0,0 };
    }
    else if (direction == DirectionLeft) {
        otherPos1 = pos + Vec3{ 0, -1,0 };
        otherPos2 = pos + Vec3{ 0, 1,0 };
    }
    else { /*if (direction == DirectionRight)*/
        otherPos1 = pos + Vec3{ 0, 1,0 };
        otherPos2 = pos + Vec3{ 0, -1,0 };
    }

    if (numInputs == 3) {
        std::shared_ptr<GameObject> paintBlobCombiner2(new PaintBlobCombiner(otherPos1, GetBlockSize(), numInputs, direction, false, TileTypePaintBlobContainer2));
        paintBlobCombiner2->SetParentObject(paintBlobCombiner);
        paintBlobCombiner->AddOtherPart(paintBlobCombiner2);    AddGameObjectAtPos(paintBlobCombiner2, otherPos1.x, otherPos1.y);
        AddGameObjectAtPos(paintBlobCombiner2, otherPos1.x, otherPos1.y);
        std::shared_ptr<GameObject> paintBlobCombiner3(new PaintBlobCombiner(otherPos2, GetBlockSize(), numInputs, direction, false, TileTypePaintBlobContainer2));
        paintBlobCombiner3->SetParentObject(paintBlobCombiner);
        paintBlobCombiner->AddOtherPart(paintBlobCombiner3);
        AddGameObjectAtPos(paintBlobCombiner3, otherPos2.x, otherPos2.y);
    }
    if (numInputs == 2) {
        std::shared_ptr<GameObject> paintBlobCombiner2(new PaintBlobCombiner(otherPos1, GetBlockSize(), numInputs, direction, false, TileTypePaintBlobContainer2));
        paintBlobCombiner2->SetParentObject(paintBlobCombiner);
        paintBlobCombiner->AddOtherPart(paintBlobCombiner2);
        AddGameObjectAtPos(paintBlobCombiner2, otherPos1.x, otherPos1.y);
    }
    
    return true;
}


void World::OnUpdate()
{
    // do inputs
    m_Input->CheckInputPresses();
    
    m_ZoomAmount -= m_Input->GetChangeZoom();
    if (m_ZoomAmount >= ZOOM_MAX)
        m_ZoomAmount = ZOOM_MAX;
    if (m_ZoomAmount <= ZOOM_MIN) 
        m_ZoomAmount = ZOOM_MIN;
    
    m_Position += m_Input->GetSpeed() + (m_Input->GetSpeed() * ((float)m_ZoomAmount/10));
    m_Rotation += m_Input->GetRotationChange();
    
   
    int WIDTH, HEIGHT;
    glfwGetWindowSize(m_Window, &WIDTH, &HEIGHT);
    
    float zoomedWidth = (WIDTH / 2) + (m_ZoomAmount * (WIDTH / 20));
    float zoomedHeight = (HEIGHT / 2) + (m_ZoomAmount * (HEIGHT / 20));

    float mousePosX = ((((m_Input->GetMousePosX() / (WIDTH / 2)) * zoomedWidth) - zoomedWidth) - m_Position.x) / m_BlockSize;
    float mousePosY = -1 * ((((m_Input->GetMousePosY() / (HEIGHT / 2)) * zoomedHeight) - zoomedHeight) + m_Position.y) / m_BlockSize;

    if (mousePosX < 0) mousePosX = floor(mousePosX);
    else mousePosX = floor(mousePosX);
    if (mousePosY < 0) mousePosY = floor(mousePosY);
    else mousePosY = floor(mousePosY);

    if (m_Input->GetLeftMouseDown())
    {
        switch (m_Input->GetLastNumPressed())
        {
            case(1): AddBelt((BeltType)std::max(std::min((BeltTypeYellow + m_Input->m_SecondNumPressed - 1), 2), 0), { mousePosX, mousePosY, 1 }, (Direction)m_Input->GetDirection()); break;
            case(2):AddPaintBlobCombiner(Vec3{ mousePosX, mousePosY,1 }, (Direction)m_Input->GetDirection(), std::max(std::min(m_Input->m_SecondNumPressed+1, 3), 2));break;
            case(3): AddPaintBlob(Vec4{ 0.0f, 1.0f, 1.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(4): AddPaintBlob(Vec4{ 1.0f, 0.0f, 1.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(5): AddPaintBlob(Vec4{ 1.0f, 1.0f, 0.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(6): AddPaintBlob(Vec4{ 0.0f, 0.0f, 0.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            case(7): AddPaintBlob(Vec4{ .6f, .4f, .4f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2); break;
            default: std::cout << "No tile for that number yet" << std::endl;
        }
    }
    if (m_Input->GetRightMouseDown())
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
                if (belt->getNextObject() == NULL)
                    heads.push_back(belt);
                belt->UpdatedThisFrame = false;
            }
        }
    }

    // update the heads. This will update every belt in that chain
    for (int i = 0; i < heads.size(); i++)
    {
        if (heads[i]->getNextObject() == NULL)
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
    m_Input->Reset();
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
