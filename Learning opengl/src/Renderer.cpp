#pragma once
#include "Renderer.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <array>

#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

#include "PaintBlobCombiner.h"

std::vector<unsigned int> makeIndices(int numQuads)
{
    std::vector<unsigned int> indices((numQuads * 6));

    int pos = 0;
    for (int i = 0; i < numQuads * 4; i += 4)
    {   // should be 0,1,2,2,3,0,
        indices[pos++] = i;
        indices[pos++] = i + 1;
        indices[pos++] = i + 2;
        indices[pos++] = i + 2;
        indices[pos++] = i + 3;
        indices[pos++] = i;
    }

    return indices;
}

static std::vector<Vertex> CreateQuad(float textureID, float size, Direction direction, float tileSize, Vec3 pos, Vec4 color, bool offset=false)
{
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;

    std::vector<Vertex> v(4);

    if (textureID == TileTypePaintBlob || textureID == TileTypePaintBlobShading)
    {   // if its a paint blob
        x = (float)(int)x;
        y = (float)(int)y;
        x -= (size/2.0f);
        y -= (size/2.0f);
    }

    if (offset)
    {
        x -= (size - tileSize)/2;
        y -= (size - tileSize)/2;
    }


    v[0].Position = { x, y , z };
    v[0].TexIndex = textureID;
    v[0].BlobColor = color;

    v[1].Position = { x + size, y, z};
    v[1].TexIndex = textureID;
    v[1].BlobColor = color;

    v[2].Position = { x + size, y + size, z };
    v[2].TexIndex = textureID;
    v[2].BlobColor = color;

    v[3].Position = { x, y + size, z };
    v[3].TexIndex = textureID;
    v[3].BlobColor = color;

    if (direction == DirectionUp)
    {
        v[0].TexCoords = { 0.0f, 0.0f };
        v[1].TexCoords = { 1.0f, 0.0f };
        v[2].TexCoords = { 1.0f, 1.0f };
        v[3].TexCoords = { 0.0f, 1.0f };
    }
    if (direction == DirectionRight)
    {
        v[0].TexCoords = { 0.0f, 0.0f };
        v[3].TexCoords = { 1.0f, 0.0f };
        v[2].TexCoords = { 1.0f, 1.0f };
        v[1].TexCoords = { 0.0f, 1.0f };
    }
    if (direction == DirectionDown)
    {
        v[2].TexCoords = { 0.0f, 0.0f };
        v[3].TexCoords = { 1.0f, 0.0f };
        v[0].TexCoords = { 1.0f, 1.0f };
        v[1].TexCoords = { 0.0f, 1.0f };
    }
    if (direction == DirectionLeft)
    {
        v[2].TexCoords = { 0.0f, 0.0f };
        v[1].TexCoords = { 1.0f, 0.0f };
        v[0].TexCoords = { 1.0f, 1.0f };
        v[3].TexCoords = { 0.0f, 1.0f };
    }
    return v;

}

Renderer::Renderer(int tileSize)
    :m_TileSize(tileSize)
{
    m_MAXNUMQUADS = 2000000;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * m_MAXNUMQUADS * 4); // max amount of vertices

    m_Layout = std::make_unique<VertexBufferLayout>();
    m_Layout->Push<float>(3);
    m_Layout->Push<float>(2);
    m_Layout->Push<float>(1);
    m_Layout->Push<float>(4);
    m_VertexArray->AddBuffer(*m_VertexBuffer, *m_Layout);

    m_Shader = std::make_unique<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");
    m_Shader->Bind();

    for (int i = 0; i < TileTypeNumTileTypes; i++)
    {
        char fileName[128] = "res/textures/game textures png/";
        strcat_s(fileName, WorldTile::TileTypeToString((TileType)i));
        m_Textures.push_back(std::make_unique<Texture>(fileName));
    }

}

void Renderer::Clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );
}

void Renderer::Draw() const
{
    m_Shader->Bind();
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::AddQuad(PaintBlob& PaintBlob)
{
    m_AllQuads.push_back(CreateQuad((float)PaintBlob.GetTile()->GetType(), (float)PaintBlob.GetSize(), PaintBlob.GetDirection(), (float)m_TileSize, PaintBlob.GetPos()* (float)m_TileSize, PaintBlob.ConvertToRGB()));
}

void Renderer::AddQuad(Belt& belt)
{
    m_AllQuads.push_back(CreateQuad((float)belt.GetTile()->GetType(), (float)belt.GetSize(), belt.GetDirection(), (float)m_TileSize, belt.GetPos() * (float)m_TileSize, belt.GetTile()->GetColor()));
}


void Renderer::AddQuad(float textureID, float size, Direction direction, Vec3 pos, Vec4 color)
{
    m_AllQuads.push_back(CreateQuad(textureID, size, direction, (float)m_TileSize, pos * size, color));
}

void Renderer::AddQuadOffset(float textureID, float size, Direction direction, Vec3 pos, float tileSize, Vec4 color)
{
    m_AllQuads.push_back(CreateQuad(textureID, size, direction, tileSize, pos * tileSize, color, true));
}


void Renderer::AddQuad(std::shared_ptr<GameObject> gameObject, Vec4 color)
{
    m_AllQuads.push_back(CreateQuad((float)gameObject->GetTile()->GetType(), (float)gameObject->GetSize(), gameObject->GetDirection(), (float)m_TileSize, gameObject->GetPos() * (float)m_TileSize, color));
}

void Renderer::DrawDrawer(World& world, int width, int height)
{
    float drawerBlockSize = (float)world.GetBlockSize();
    float itemBlockSize = world.GetBlockSize() * .75f;

    float startDrawX = -1 * (width/2.0f);
    float startDrawY = -1 * (height/2.0f);

    float percentOffset = .2f;

    startDrawX *= (1.0f - percentOffset);
    startDrawY *= (1.0f - percentOffset);

    startDrawX /= drawerBlockSize;
    startDrawY /= drawerBlockSize;

    // background for first number selection drawer
    for (float i = 0; i < 9; i++) {
        AddQuad(TileTypeDrawerBackground, drawerBlockSize, DirectionUp, { startDrawX + i, startDrawY, 1 });
    }
    // first number selection drawer
    AddQuadOffset(TileTypeStraightBelt, itemBlockSize, DirectionUp, Vec3{ startDrawX, startDrawY, 1 }, drawerBlockSize);
    AddQuadOffset(TileTypeYellowArrow, itemBlockSize, DirectionUp, Vec3{startDrawX, startDrawY, 1}, drawerBlockSize);
    if (world.GetInput()->GetLastNumPressed() == 1) 
        AddQuadOffset((float)TileTypeYellowArrow + (BeltType)(std::max(std::min((BeltTypeYellow + world.GetInput()->m_SecondNumPressed - 1), 2), 0)), itemBlockSize, DirectionUp, Vec3{startDrawX, startDrawY, 1}, drawerBlockSize);
    AddQuadOffset(TileTypePaintBlobContainer1, itemBlockSize, DirectionUp, Vec3{ startDrawX + 1, startDrawY, 1 }, drawerBlockSize);

    if (world.GetInput()->GetLastNumPressed() > 0) // highlight pressed number
        AddQuad(TileTypeDrawerBackgroundSelected, drawerBlockSize, DirectionUp, { startDrawX + world.GetInput()->GetLastNumPressed() - 1, startDrawY, 1 });
    
    // second number selection drawer
    if (world.GetInput()->GetWaitingForSecondNumber())
    {
        switch (world.GetInput()->GetLastNumPressed())
        {
        case 1:
            for (float i = 0; i < 3; i++) {
                AddQuad(TileTypeDrawerBackground, drawerBlockSize, DirectionUp, { startDrawX + i, startDrawY + 1, 1 });
            }
            AddQuadOffset(TileTypeStraightBelt, itemBlockSize, DirectionUp, Vec3{ startDrawX, startDrawY + 1, 1 }, drawerBlockSize);
            AddQuadOffset(TileTypeYellowArrow, itemBlockSize, DirectionUp, Vec3{ startDrawX, startDrawY + 1, 1 }, drawerBlockSize);
            AddQuadOffset(TileTypeStraightBelt, itemBlockSize, DirectionUp, Vec3{ startDrawX + 1, startDrawY + 1, 1 }, drawerBlockSize);
            AddQuadOffset(TileTypeOrangeArrow, itemBlockSize, DirectionUp, Vec3{ startDrawX + 1, startDrawY + 1, 1 }, drawerBlockSize);
            AddQuadOffset(TileTypeStraightBelt, itemBlockSize, DirectionUp, Vec3{ startDrawX + 2, startDrawY + 1, 1 }, drawerBlockSize);
            AddQuadOffset(TileTypeRedArrow, itemBlockSize, DirectionUp, Vec3{ startDrawX + 2, startDrawY + 1, 1 }, drawerBlockSize);
            break;
        case 2:
            for (float i = 0; i < 2; i++) {
                AddQuad(TileTypeDrawerBackground, drawerBlockSize, DirectionUp, { startDrawX + i, startDrawY + 1, 1 });
            }
            AddQuadOffset(TileTypePaintBlobContainer1, itemBlockSize, DirectionUp, Vec3{ startDrawX, startDrawY + 1, 1 }, drawerBlockSize);
            AddQuadOffset(TileTypePaintBlobContainer2, itemBlockSize, DirectionUp, Vec3{ startDrawX + 1, startDrawY + 1, 1 }, drawerBlockSize);
        default:;
        }
        
    }
}

void Renderer::DrawWorld(World& world, int width, int height)
{
    // draw background
    int size = world.GetBlockSize();
    std::vector<glm::vec3> OnScreenPositions;

    float zoomedWidth = (float)((width / 2) + (world.GetZoomAmount() * (width / 20)));
    float zoomedHeight = (float)((height / 2) + (world.GetZoomAmount() * (height / 20)));

    int startDrawX = (int)(-1 * ((world.GetPosition().x / size) + (zoomedWidth / size))) - 1;
    int startDrawY = (int)(-1 * ((world.GetPosition().y / size) + (zoomedHeight / size))) - 1;

    int amountToDrawX = (int)(zoomedWidth / size) * 2;
    int amountToDrawY = (int)(zoomedHeight / size) * 2;

    int extraQuads = 3;

    for (float x = (float)startDrawX; x < startDrawX + amountToDrawX + extraQuads; x++) {
        for (float y = (float)startDrawY; y < startDrawY + amountToDrawY + extraQuads; y++) {
            OnScreenPositions.push_back(glm::vec3(x, y, 1));
            std::shared_ptr<WorldBackgroundTile> tile = world.GetWorldBackgroundTileAtPos(x, y);
            if (tile != nullptr)
                AddQuad(tile->GetTileType(), (float)size, tile->GetDirection(), Vec3{x, y, 1});
            else
                AddQuad(TileTypeGrass, (float)size, DirectionUp, Vec3{x, y, 1});
        }
    }
   

    std::vector<std::shared_ptr<GameObject>> blobsOnBelts;
    // draw world tiles
    for (unsigned int i = 0; i < OnScreenPositions.size(); i++)
    {
        // draw belt if it exists
        std::shared_ptr<Belt> belt = world.GetBeltAtPos(OnScreenPositions[i].x, OnScreenPositions[i].y);
        if (belt != NULL)
        { // there is a belt on this square
            AddQuad(belt);
            if (belt->GetTile()->GetType() == TileTypeStraightBelt) // draw arrow if it is not a turn belt
                AddQuad((float)belt->GetArrowTile(), (float)size, belt->GetDirection(), belt->GetArrowPos());

            // add objects on the belt to draw to the list of things to draw
            for (unsigned int i = 0; i < belt->GetAllObjects().size(); i++)
            {
                blobsOnBelts.push_back(belt->GetAllObjects()[i]);
            }
        }

        // draw paint blob combiners
        std::shared_ptr<GameObject> gameObject = world.GetGameObjectAtPos(OnScreenPositions[i].x, OnScreenPositions[i].y);
        if (gameObject != NULL)
            AddQuad(gameObject);


        // draw all other paint blobs
        std::shared_ptr<PaintBlob> paintBlob = world.GetPaintBlobAtPos(OnScreenPositions[i].x, OnScreenPositions[i].y);
        if (paintBlob != NULL)
        {
            AddQuad(paintBlob, paintBlob->ConvertToRGB());
            paintBlob->GetTile()->SetType(TileTypePaintBlobShading);
            AddQuad(paintBlob, paintBlob->ConvertToRGB());
            paintBlob->GetTile()->SetType(TileTypePaintBlob);
        }

        // draw trucks
        std::shared_ptr<Truck> truck = world.GetTruckAtPos(OnScreenPositions[i].x, OnScreenPositions[i].y);
        if (truck != NULL)
            AddQuad(TileTypeTruck, size, truck->GetDirection(), truck->GetPos(), {0,0,0,0});

        // draw IndoorAreaAddButton
        std::shared_ptr<IndoorAreaAddButton> indoorAreaAddButton = world.GetIndoorAreaAddButtonAtPos(OnScreenPositions[i].x, OnScreenPositions[i].y);
        if (indoorAreaAddButton != NULL)
			AddQuad(TileTypePlusButton, size, DirectionUp, indoorAreaAddButton->GetPos(), {0,0,0,0});
    }


    // draw the belt paint blobs
    for (unsigned int i = 0; i < blobsOnBelts.size(); i++)
    {
        if (PaintBlob* b = dynamic_cast<PaintBlob*>(blobsOnBelts[i].get())) {
            AddQuad(blobsOnBelts[i], b->ConvertToRGB());
            blobsOnBelts[i]->GetTile()->SetType(TileTypePaintBlobShading);
            AddQuad(blobsOnBelts[i], b->ConvertToRGB());
            blobsOnBelts[i]->GetTile()->SetType(TileTypePaintBlob);
        }
    }
}

void Renderer::RenderScene(int width, int height, World& world, glm::mat4& mvp)
{
    // set dynamic vertex buffer
    std::vector<Vertex> vertices(m_AllQuads.size() * 4);
    if (m_MAXNUMQUADS < m_AllQuads.size())
    {
        std::cout << "TOO MANY QUADS" << std::endl;
        std::cout << "Need " << m_AllQuads.size() << " Quads" << std::endl;
        std::cout << "Max is " << m_MAXNUMQUADS << " Quads" << std::endl;
    }

    Vertex* buffer = vertices.data();

    for (unsigned int i = 0; i < m_AllQuads.size(); i++)
    {
        memcpy(buffer + (m_AllQuads[i].size() * i), m_AllQuads[i].data(), m_AllQuads[i].size() * sizeof(Vertex));
    }

    m_VertexBuffer->Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

    for (unsigned int i = 0; i < m_Textures.size(); i++) {
        m_Textures[i]->Bind(i);
    }

    int numSamplers = m_Textures.size();
    if (numSamplers > 32)
        std::cout << "TOO MANY TEXTURES" << std::endl;

    int* samplers = (int*)malloc(numSamplers * sizeof(int));
    for (int i = 0; i < numSamplers; i++) { samplers[i] = i; }

    m_Shader->SetUniform1iv("u_Textures", numSamplers, samplers);

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    Draw();
    free(samplers);
}

void Renderer::OnRender(int width, int height, World& world, bool beenOneSecond, bool printStuff)
{

    DrawWorld(world, width, height);

    float v1 = (float)(-1 * (width / 2) - (world.GetZoomAmount() * (width / 20)));
    float v2 = -1 * v1;
    float v3 = (float)(-1 * (height / 2) - (world.GetZoomAmount() * (height / 20)));
    float v4 = -1 * v3;

    glm::mat4 proj = glm::ortho(v1, v2, v3, v4, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), world.GetPosition());

    if (world.IS3D) {
        view = glm::rotate(view, glm::radians(world.GetRotation()), glm::vec3(1, 0, 0));
        proj = glm::perspective(glm::radians(90.0f), v2 / v4, -1.0f, 1.0f);
    }
    glm::mat4 mvp = proj * view;

    Clear();

    std::vector<unsigned int> indices = makeIndices(m_AllQuads.size());
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    
    RenderScene(width, height, world, mvp);
    if (beenOneSecond && printStuff)
    {
        std::cout << "Drawing " << GetAmountOfCurrentQuads() << "/" << GetMaxAmountOfQuads()
            << " Quads: " << ((float)GetAmountOfCurrentQuads() / GetMaxAmountOfQuads()) * 100 << "% " << std::endl;
    }

    DeleteQuads();

    // draw drawer
    DrawDrawer(world, width, height);
    v1 = (float)(-1 * (width / 2));
    v2 = -1 * v1;
    v3 = (float)(-1 * (height / 2));
    v4 = -1 * v3;
    mvp = glm::ortho(v1, v2, v3, v4, -1.0f, 1.0f);

    indices = makeIndices(m_AllQuads.size());
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    RenderScene(width, height, world, mvp);

    DeleteQuads();
}

void Renderer::DeleteQuads()
{
    m_AllQuads.clear();
}

