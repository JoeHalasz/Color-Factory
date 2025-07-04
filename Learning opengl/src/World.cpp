#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "IndoorArea.h"
#include "World.h"

World::World(GLFWwindow* window)
    :m_Window(window)
{
    if (IS3D)
        m_Position.z = -350;
    // set up inputs
    SetInput();
    // make the first indoor area
    std::shared_ptr<IndoorArea> newIndoorArea = std::make_shared<IndoorArea>();
    AddIndoorArea(newIndoorArea, DirectionUp, true);
    for (auto& row : m_TruckNodes)
    {
        for (auto& col : row.second)
        {
            std::cout << "HERE" << std::endl;
        }
    }
}

World::World(GLFWwindow* window, glm::vec3 position)
    :m_Window(window), m_Position(position)
{
    if (IS3D)
        m_Position.z = -350;
    // set up inputs
    SetInput();
    // make the first indoor area
    std::shared_ptr<IndoorArea> newIndoorArea = std::make_shared<IndoorArea>();
    AddIndoorArea(newIndoorArea, DirectionUp, true);

}

World::~World()
{
}


int World::GetBeltDirectionAt(int x, int y)
{
    std::shared_ptr<Belt> belt = GetBeltAtPos((float)x, (float)y);
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
    std::shared_ptr<PaintBlob> paintBlob = std::make_shared<PaintBlob>(pos + .5f, (int)(size * m_BlockSize), BlobColor);
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

bool World::AddTruck()
{
    std::shared_ptr<TruckNode> node;
    std::shared_ptr<TruckStop> stop;

    for (auto& row : m_TruckNodes)
    {
        for (auto& col : row.second)
        {
			node = col.second;
        }
    }

    for (auto& row : m_TruckStops)
    {
        for (auto& col : row.second)
        {
            stop = col.second;
            break;
        }
        break;
    }
    
    std::shared_ptr<Truck> truck = std::make_shared<Truck>(node);
    truck->SetTargetStop(stop);
    AddTruckAtPos(truck, truck->GetPos().x, truck->GetPos().y);
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

    std::shared_ptr<Belt> belt = std::make_shared<Belt>(WorldTile(TileTypeStraightBelt), pos, m_BlockSize, direction, beltColor);

    if (direction == DirectionUp)
    {
        int leftBelt = GetBeltDirectionAt((int)pos.x - 1, (int)pos.y);
        int rightBelt = GetBeltDirectionAt((int)pos.x + 1, (int)pos.y);
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
        int upBelt = GetBeltDirectionAt((int)pos.x, (int)pos.y + 1);
        int downBelt = GetBeltDirectionAt((int)pos.x, (int)pos.y - 1);
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
        int rightBelt = GetBeltDirectionAt((int)pos.x + 1, (int)pos.y);
        int leftBelt = GetBeltDirectionAt((int)pos.x - 1, (int)pos.y);
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
        int downBelt = GetBeltDirectionAt((int)pos.x, (int)pos.y - 1);

        int upBelt = GetBeltDirectionAt((int)pos.x, (int)pos.y + 1);
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
    int x = (int)std::floor(belt->GetPos().x);
    int y = (int)std::floor(belt->GetPos().y);
    if (m_Belts.find(x) != m_Belts.end() && m_Belts[x].find(y) != m_Belts[x].end() && m_Belts[x][y] != NULL)
    {
        std::shared_ptr<Belt> otherBelt = m_Belts[x][y];
        if (otherBelt->GetBeltTypeSpeed() == belt->GetBeltTypeSpeed() && otherBelt->GetDirection() == belt->GetDirection())
			return false;
        m_Belts[x][y].reset();
        m_Belts[x].erase(y);
    }
    if (m_PaintBlobs.find(x) != m_PaintBlobs.end() && m_PaintBlobs[x].find(y) != m_PaintBlobs[x].end() && m_PaintBlobs[x][y] != NULL)
    {
        m_PaintBlobs[x][y].reset();
        m_PaintBlobs[x].erase(y);// this will only be blobs because of check at beginning of func
    }
    AddBeltAtPos(belt, belt->GetPos().x, belt->GetPos().y);
    belt->SetUpNextAndLastObject(m_Belts, m_GameObjects);

    return true;
}


bool World::AddPaintBlobCombiner(Vec3 pos, Direction direction, int numInputs)
{
    if (!NothingAtPos(pos))
        return false;
    if (!isIndoors(pos))
        return false; // this means its not indoors
    if (!NothingAtPos(Vec3{ pos.x + 1, pos.y, pos.z }))
        return false;
    if (!isIndoors(pos + Vec3{1,0,0}))
		return false; // this means its not indoors
    if (numInputs == 3)
    {
        if (!NothingAtPos(Vec3 { pos.x - 1, pos.y, pos.z }))
			return false;
        if (!isIndoors(pos + Vec3{ -1,0,0 }))
            return false; // this means its not indoors
    }

    

    std::shared_ptr<GameObject> paintBlobCombiner = std::make_shared<PaintBlobCombiner>(pos, GetBlockSize(), numInputs, direction, true);
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
        std::shared_ptr<GameObject> paintBlobCombiner2 = std::make_shared<PaintBlobCombiner>(otherPos1, GetBlockSize(), numInputs, direction, false, TileTypePaintBlobContainer2);
        paintBlobCombiner2->SetParentObject(paintBlobCombiner);
        paintBlobCombiner->AddOtherPart(paintBlobCombiner2);    AddGameObjectAtPos(paintBlobCombiner2, otherPos1.x, otherPos1.y);
        AddGameObjectAtPos(paintBlobCombiner2, otherPos1.x, otherPos1.y);
        std::shared_ptr<GameObject> paintBlobCombiner3 = std::make_shared<PaintBlobCombiner>(otherPos2, GetBlockSize(), numInputs, direction, false, TileTypePaintBlobContainer2);
        paintBlobCombiner3->SetParentObject(paintBlobCombiner);
        paintBlobCombiner->AddOtherPart(paintBlobCombiner3);
        AddGameObjectAtPos(paintBlobCombiner3, otherPos2.x, otherPos2.y);
    }
    if (numInputs == 2) {
        std::shared_ptr<GameObject> paintBlobCombiner2 = std::make_shared<PaintBlobCombiner>(otherPos1, GetBlockSize(), numInputs, direction, false, TileTypePaintBlobContainer2);
        paintBlobCombiner2->SetParentObject(paintBlobCombiner);
        paintBlobCombiner->AddOtherPart(paintBlobCombiner2);
        AddGameObjectAtPos(paintBlobCombiner2, otherPos1.x, otherPos1.y);
    }
    
    return true;
}


// this will return true if there is an indoor area in the passed in direction from the passed in pos 
bool World::HasIndoorAreaInDirection(std::shared_ptr<IndoorArea> area, Direction direction)
{
    Vec3 pos = area->GetMiddlePosition();
    float roadWidth = 6.5f;
    switch (direction) {
        case(DirectionUp): pos = pos + Vec3{ 0,area->GetDirectionRightFromMiddle() + roadWidth,0 }; break;
        case(DirectionDown): pos = pos + Vec3{ 0,-1*(area->GetDirectionRightFromMiddle() + roadWidth),0}; break;
        case(DirectionLeft): pos = pos + Vec3{ -1 * (area->GetDirectionRightFromMiddle() + roadWidth), 0,0 }; break;
        case(DirectionRight): pos = pos + Vec3{ area->GetDirectionRightFromMiddle() + roadWidth, 0,0 }; break;
    }
    std::cout << GetWorldBackgroundTileAtPos(pos.x, pos.y) << std::endl;
    if (GetWorldBackgroundTileAtPos(pos.x, pos.y) != NULL)
        return true;
    return false;
}


bool World::AddIndoorArea(std::shared_ptr<IndoorArea> lastArea, Direction directionToCreate, bool isFirst)
{
    std::shared_ptr<IndoorArea> newArea;
    int amountExtra = 6;
    if (isFirst) // if its the first area then just create it in the middle at the default size
        newArea = std::make_shared<IndoorArea>(Vec3{ 0,0,1 }, DirectionUp, m_TruckNodes, m_TruckStops, true);
    else
    {
        // check if we can expand in the direction we want to create a new area
        if (lastArea->checkExpandAvailable(directionToCreate)) {
            Vec3 newPos = lastArea->GetMiddlePosition();
            int lengthFromMiddle = 20;
            switch (directionToCreate)
            {
            case (DirectionUp): newPos = { newPos.x, newPos.y + lastArea->GetDirectionDownFromMiddle() + lengthFromMiddle + amountExtra, newPos.z }; break;
            case (DirectionDown):newPos = { newPos.x, newPos.y - lastArea->GetDirectionUpFromMiddle() - lengthFromMiddle - amountExtra, newPos.z }; break;
            case (DirectionLeft): newPos = { newPos.x + lastArea->GetDirectionRightFromMiddle() + lengthFromMiddle + amountExtra, newPos.y, newPos.z }; break;
            case (DirectionRight): newPos = { newPos.x - lastArea->GetDirectionLeftFromMiddle() - lengthFromMiddle - amountExtra, newPos.y, newPos.z }; break;
            }
            newArea = std::make_shared<IndoorArea>(newPos, directionToCreate, m_TruckNodes, m_TruckStops, false, lengthFromMiddle, lengthFromMiddle, lengthFromMiddle, lengthFromMiddle);

            lastArea->AddDirectionToOtherAreas(directionToCreate);
        }
        else
            return false;
    }
    std::shared_ptr<WorldBackgroundTile> tile;
    std::shared_ptr<WorldBackgroundTile> tile2;
    Vec3 middle = newArea->GetMiddlePosition();
    // add the new squares of that area to the world based on newArea.size(doesnt exist) info TODO
    for (float x = middle.x - newArea->GetDirectionLeftFromMiddle(); x < middle.x + newArea->GetDirectionRightFromMiddle(); x++)
    {
        for (float y = middle.y - newArea->GetDirectionDownFromMiddle(); y < middle.y + newArea->GetDirectionUpFromMiddle(); y++)
        {
            tile = std::make_shared<WorldBackgroundTile>(TileTypeBackgroundIndoor, DirectionUp);
            AddWorldBackgroundTileAtPos(tile, x, y);
        }
    }
    // add the walls
    tile = std::make_shared<WorldBackgroundTile>(TileTypeWall, DirectionLeft);
    tile2 = std::make_shared<WorldBackgroundTile>(TileTypeWall, DirectionRight);
    for (float x = middle.x - newArea->GetDirectionDownFromMiddle() + 1; x < middle.x + newArea->GetDirectionUpFromMiddle(); x++)
    {
        AddWorldBackgroundTileAtPos(tile, x, middle.y - newArea->GetDirectionDownFromMiddle());
        AddWorldBackgroundTileAtPos(tile2, x, middle.y + newArea->GetDirectionDownFromMiddle());
    }
    tile = std::make_shared<WorldBackgroundTile>(TileTypeWall, DirectionDown);
    tile2 = std::make_shared<WorldBackgroundTile>(TileTypeWall, DirectionUp);
    for (float y = middle.y - newArea->GetDirectionLeftFromMiddle() + 1; y < middle.y + newArea->GetDirectionRightFromMiddle(); y++)
    {
        AddWorldBackgroundTileAtPos(tile, middle.x - newArea->GetDirectionLeftFromMiddle(), y);
        AddWorldBackgroundTileAtPos(tile2, middle.x + newArea->GetDirectionRightFromMiddle(), y);
    }
    // add the corners
    tile = std::make_shared<WorldBackgroundTile>(TileTypeWallCorner, DirectionUp);
    AddWorldBackgroundTileAtPos(tile, middle.x + newArea->GetDirectionRightFromMiddle(), middle.y - newArea->GetDirectionUpFromMiddle());
    tile = std::make_shared<WorldBackgroundTile>(TileTypeWallCornerBackwards, DirectionDown);
    AddWorldBackgroundTileAtPos(tile, middle.x + newArea->GetDirectionRightFromMiddle(), middle.y + newArea->GetDirectionDownFromMiddle());
    tile = std::make_shared<WorldBackgroundTile>(TileTypeWallCorner, DirectionDown);
    AddWorldBackgroundTileAtPos(tile, middle.x - newArea->GetDirectionLeftFromMiddle(), middle.y + newArea->GetDirectionDownFromMiddle());
    tile = std::make_shared<WorldBackgroundTile>(TileTypeWallCornerBackwards, DirectionUp);
    AddWorldBackgroundTileAtPos(tile, middle.x - newArea->GetDirectionLeftFromMiddle(), middle.y - newArea->GetDirectionUpFromMiddle());

    // add the roads
    tile = std::make_shared<WorldBackgroundTile>(TileTypeRoad, DirectionLeft);
    tile2 = std::make_shared<WorldBackgroundTile>(TileTypeRoadMiddle, DirectionLeft);
    for (float x = middle.x - newArea->GetDirectionDownFromMiddle() - amountExtra + 1; x < middle.x + newArea->GetDirectionUpFromMiddle() + amountExtra; x++)
    {
        for (int i = 1; i < 6; i++)
        {
            AddWorldBackgroundTileAtPos(tile, x, middle.y - newArea->GetDirectionDownFromMiddle() - i);
            AddWorldBackgroundTileAtPos(tile, x, middle.y + newArea->GetDirectionDownFromMiddle() + i);
        }
        AddWorldBackgroundTileAtPos(tile2, x, middle.y - newArea->GetDirectionDownFromMiddle() - 3);
        AddWorldBackgroundTileAtPos(tile2, x, middle.y + newArea->GetDirectionDownFromMiddle() + 3);
    }
    tile = std::make_shared<WorldBackgroundTile>(TileTypeRoad, DirectionUp);
    tile2 = std::make_shared<WorldBackgroundTile>(TileTypeRoadMiddle, DirectionUp);
    for (float y = middle.y - newArea->GetDirectionLeftFromMiddle() - amountExtra + 1; y < middle.y + newArea->GetDirectionUpFromMiddle() + amountExtra; y++)
    {
        for (int i = 1; i < 6; i++)
        {
            AddWorldBackgroundTileAtPos(tile, middle.x - newArea->GetDirectionDownFromMiddle() - i, y);
            AddWorldBackgroundTileAtPos(tile, middle.x + newArea->GetDirectionDownFromMiddle() + i, y);
        }
        // add middle parts
        AddWorldBackgroundTileAtPos(tile2, middle.x - newArea->GetDirectionDownFromMiddle() - 3, y);
        AddWorldBackgroundTileAtPos(tile2, middle.x + newArea->GetDirectionDownFromMiddle() + 3, y);
    }
    // fix some of the middle parts

    // add the middle corners if there wasnt a corner already there
    tile = std::make_shared<WorldBackgroundTile>(TileTypeRoad, DirectionUp);
    for (int i = 1; i < 6; i++)
    {
        AddWorldBackgroundTileAtPos(tile, middle.x - newArea->GetDirectionLeftFromMiddle() - 3, middle.y - newArea->GetDirectionUpFromMiddle() - i);
        AddWorldBackgroundTileAtPos(tile, middle.x + newArea->GetDirectionRightFromMiddle() + 3, middle.y - newArea->GetDirectionUpFromMiddle() - i);
        AddWorldBackgroundTileAtPos(tile, middle.x - newArea->GetDirectionLeftFromMiddle() - 3, middle.y + newArea->GetDirectionDownFromMiddle() + i);
        AddWorldBackgroundTileAtPos(tile, middle.x + newArea->GetDirectionRightFromMiddle() + 3, middle.y + newArea->GetDirectionDownFromMiddle() + i);
    }

    // add the add new Indoor Area buttons
    if (!HasIndoorAreaInDirection(newArea, DirectionDown)) {
        std::shared_ptr <IndoorAreaAddButton> newButton = std::make_shared<IndoorAreaAddButton>(newArea, DirectionUp, Vec3{ middle.x, middle.y - newArea->GetDirectionUpFromMiddle() - 5 , 1 }, *this);
        AddIndoorAreaAddButtonAtPos(newButton, newButton->GetPos().x, newButton->GetPos().y);
    }
    if (!HasIndoorAreaInDirection(newArea, DirectionUp)) {
        std::shared_ptr <IndoorAreaAddButton> newButton = std::make_shared<IndoorAreaAddButton>(newArea, DirectionDown, Vec3{ middle.x, middle.y + newArea->GetDirectionDownFromMiddle() + 5 , 1 }, *this);
        AddIndoorAreaAddButtonAtPos(newButton, newButton->GetPos().x, newButton->GetPos().y);
    }
    if (!HasIndoorAreaInDirection(newArea, DirectionRight)) {
        std::shared_ptr <IndoorAreaAddButton> newButton = std::make_shared<IndoorAreaAddButton>(newArea, DirectionRight, Vec3{ middle.x + newArea->GetDirectionLeftFromMiddle() + 5 ,middle.y , 1 }, *this);
        AddIndoorAreaAddButtonAtPos(newButton, newButton->GetPos().x, newButton->GetPos().y);
    }
    if (!HasIndoorAreaInDirection(newArea, DirectionLeft)) {
        std::shared_ptr <IndoorAreaAddButton> newButton = std::make_shared<IndoorAreaAddButton>(newArea, DirectionLeft, Vec3{ middle.x - newArea->GetDirectionRightFromMiddle() - 5 ,middle.y , 1 }, *this);
        AddIndoorAreaAddButtonAtPos(newButton, newButton->GetPos().x, newButton->GetPos().y);
    }
    // find any other indoor area add buttons around it that shouldnt be there anymore and delete them
    if (GetIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x, newArea->GetMiddlePosition().y - newArea->GetDirectionUpFromMiddle() - 1) != NULL)
        DeleteIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x, newArea->GetMiddlePosition().y - newArea->GetDirectionUpFromMiddle() - 1);
    if (GetIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x, newArea->GetMiddlePosition().y + newArea->GetDirectionUpFromMiddle() + 1) != NULL)
        DeleteIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x, newArea->GetMiddlePosition().y + newArea->GetDirectionUpFromMiddle() + 1);
    if (GetIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x + newArea->GetDirectionLeftFromMiddle() + 1, newArea->GetMiddlePosition().y) != NULL)
        DeleteIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x + newArea->GetDirectionLeftFromMiddle() + 1, newArea->GetMiddlePosition().y);
    if (GetIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x - newArea->GetDirectionLeftFromMiddle() - 1, newArea->GetMiddlePosition().y) != NULL)
        DeleteIndoorAreaAddButtonAtPos(newArea->GetMiddlePosition().x - newArea->GetDirectionLeftFromMiddle() - 1, newArea->GetMiddlePosition().y);

    
    // add the area
    m_IndoorAreas.push_back(newArea);
    
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

    float zoomedWidth  = (float)((WIDTH  / 2) + (m_ZoomAmount * (WIDTH  / 20)));
    float zoomedHeight = (float)((HEIGHT / 2) + (m_ZoomAmount * (HEIGHT / 20)));

    float mousePosX = (float)(     ((((m_Input->GetMousePosX() / (WIDTH  / 2)) * zoomedWidth ) - zoomedWidth ) - m_Position.x) / m_BlockSize);
    float mousePosY = (float)(-1 * ((((m_Input->GetMousePosY() / (HEIGHT / 2)) * zoomedHeight) - zoomedHeight) + m_Position.y) / m_BlockSize);
    
    // adjust for bottom and top of the screen making the calculation off (not sure why this is needed but without it the mouse is off a little at the top of the screen)
    mousePosY += (((HEIGHT - m_Input->GetMousePosY()) / HEIGHT)/2) + (std::max(0.0f,((zoomedHeight - HEIGHT)/HEIGHT)))/2;

    mousePosX = floor(mousePosX);
    mousePosY = floor(mousePosY);

    if (m_Input->GetLeftMouseDown())
    {
        // if there is a indoor background square on the mouse pos place something
        if ((isIndoors(Vec3{ mousePosX, mousePosY, 1 }))) {
            switch (m_Input->GetLastNumPressed())
            {
                case(1): AddBelt((BeltType)std::max(std::min((BeltTypeYellow + m_Input->m_SecondNumPressed - 1), 2), 0), { mousePosX, mousePosY, 1 }, (Direction)m_Input->GetDirection()); break;
                case(2): AddPaintBlobCombiner(Vec3{ mousePosX, mousePosY,1 }, (Direction)m_Input->GetDirection(), std::max(std::min(m_Input->m_SecondNumPressed + 1, 3), 2)); break;
                case(3): AddIndoorArea(GetIndoorAreas()[GetIndoorAreas().size() - 1], (Direction)(std::max(std::min(m_Input->m_SecondNumPressed, 4), 1) - 1), false); break;
                case(4): AddPaintBlob(Vec4{ 0.0f, 1.0f, 1.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2f); break;
                case(5): AddPaintBlob(Vec4{ 1.0f, 0.0f, 1.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2f); break;
                case(6): AddPaintBlob(Vec4{ 1.0f, 1.0f, 0.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2f); break;
                case(7): AddPaintBlob(Vec4{ 0.0f, 0.0f, 0.0f, 0.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2f); break;
                case(8): AddPaintBlob(Vec4{ .6f, .4f, .4f, 1.0f }, Vec3{ mousePosX, mousePosY, 1 }, .2f); break;
                case(9): AddTruck(); break;
                default: std::cout << "No tile for that number yet" << std::endl;
            }
        }
        else {
            // check for button presses instead
            std::shared_ptr<IndoorAreaAddButton> button = GetIndoorAreaAddButtonAtPos(mousePosX, mousePosY, true);
            if (button != NULL)
            {
                // button was pressed, add an indoor area
                AddIndoorArea(button->GetConnectedTo(), (Direction)InvertDirection(button->GetDirection()));
            }
        }
    }
    if (m_Input->GetRightMouseDown())
    {
        DeleteAllAtPos(Vec3{ mousePosX, mousePosY, 1 });
    }

    std::vector<std::shared_ptr<Belt>> heads;

    int x, y;
    // find all the belts that have a head and reset UpdatedThisFrame
    for (auto& row : m_Belts)
    {
        x = row.first;
        for (auto& col : row.second)
        {
            y = col.first;
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
    for (unsigned int i = 0; i < heads.size(); i++)
    {
        if (heads[i]->getNextObject() == NULL) // it will not have a next if its a head
            heads[i]->Update(); // this will call all the belts in the chain to update
    }


    // update all the belts that dont have a head
    for (auto& row : m_Belts)
    {
        x = row.first;
        for (auto& col : row.second)
        {
            y = col.first;
            std::shared_ptr<Belt> belt = col.second;
            if (belt != NULL)
                if (!belt->UpdatedThisFrame)
                    belt->Update(); // this will call all the belts in the chain to update
            
        }
    }

    // update all the game objects
    for (auto& row : m_PaintBlobs)
    {
        x = row.first;
        for (auto& col : row.second)
        {
            y = col.first;
            std::shared_ptr<PaintBlob> paintBlobs = col.second;
            if (paintBlobs != NULL)
            {
                // paintBlob.Update();
            }

        }
    }
    
    // update all the trucks
    for (auto& row : m_Trucks)
    {
        x = row.first;
        for (auto& col : row.second)
        {
            y = col.first;
            std::shared_ptr<Truck> truck = col.second;
            if (truck != NULL)
            {
                truck->Update();
            }

        }
    }

    m_Input->Reset();
}


bool World::BeltCanBeMade(Vec3 pos, BeltType beltColor, Direction direction)
{
    std::shared_ptr<Belt> belt = GetBeltAtPos(pos.x, pos.y);
    std::shared_ptr<PaintBlob> paintBlob = GetPaintBlobAtPos(pos.x, pos.y);

    if (!isIndoors(pos))
		return false; // this means its not indoors

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
    int x = (int)std::floor(pos.x);
    int y = (int)std::floor(pos.y);
    
    if (m_Belts.find(x) != m_Belts.end())
    {
        if (m_Belts[x].find(y) != m_Belts[x].end())
        {
            m_Belts[x][y].reset();
			m_Belts[x].erase(y); 
		}
	}
    if (m_PaintBlobs.find(x) != m_PaintBlobs.end())
    {
        if (m_PaintBlobs[x].find(y) != m_PaintBlobs[x].end())
        {
            m_PaintBlobs[x][y].reset();
            m_PaintBlobs[x].erase(y);
        }
    }

    if (GetGameObjectAtPos(pos.x, pos.y) != NULL)
    {
        std::shared_ptr<GameObject> objectHere = GetGameObjectAtPos(pos.x, pos.y);
        if (!objectHere->GetIsBasePart())
        {
            for (unsigned int j = 0; j < objectHere->GetOtherParts().size(); j++)
            {
                x = (int)objectHere->GetOtherParts()[j]->GetPos().x;
                y = (int)objectHere->GetOtherParts()[j]->GetPos().y;
                if (m_GameObjects.find(x) != m_GameObjects.end())
                {
                    if (m_GameObjects[x].find(y) != m_GameObjects[x].end())
                    {
                        m_GameObjects[x][y].reset();
                        m_GameObjects[x].erase(y);
                    }
                }
            }
        }
        else {
            std::shared_ptr<GameObject> parent = objectHere->GetParentObject();
            for (unsigned int j = 0; j < parent->GetOtherParts().size(); j++)
            {
                x = (int)parent->GetOtherParts()[j]->GetPos().x;
                y = (int)parent->GetOtherParts()[j]->GetPos().y;
                if (m_GameObjects.find(x) != m_GameObjects.end())
                {
                    if (m_GameObjects[x].find(y) != m_GameObjects[x].end())
                    {
                        m_GameObjects[x][y].reset();
                        m_GameObjects[x].erase(y);
                    }
                }
			}
            x = (int)objectHere->GetParentObject()->GetPos().x;
            y = (int)objectHere->GetParentObject()->GetPos().y;
            if (m_GameObjects.find(x) != m_GameObjects.end())
            {
                if (m_GameObjects[x].find(y) != m_GameObjects[x].end())
                {
                    m_GameObjects[x][y].reset();
                    m_GameObjects[x].erase(y); // might have to reset [x][y] before this also
                }
            }
        }
    }
    x = (int)std::floor(pos.x);
    y = (int)std::floor(pos.y);
    if (m_GameObjects.find(x) != m_GameObjects.end())
    {
        if (m_GameObjects[x].find(y) != m_GameObjects[x].end())
        {
            m_GameObjects[x][y].reset();
            m_GameObjects[x].erase(y); // might have to reset [x][y] before this also
        }
    }
}
