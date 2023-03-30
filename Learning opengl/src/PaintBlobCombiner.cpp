#include "PaintBlobCombiner.h"

PaintBlobCombiner::PaintBlobCombiner(Vec3 pos, int size, int numInputs, Direction direction, bool isBasePart, TileType type)
	: m_NumToCombine(numInputs)
{
	std::cout << numInputs << std::endl;
	SetPos(pos);
	SetSize(size);
	SetDirection(direction);
	SetIsBasePart(isBasePart);
	WorldTile tile = WorldTile(type);
	SetTile(&tile);
}

Vec4 CombineColors(std::vector<PaintBlob*> allColors)
{
	if (allColors.size() == 0)
		return { 0,0,0,0 };
	
	Vec4 newColor = { 0,0,0,0 };
	for (unsigned int i = 0; i < allColors.size(); i++)
	{
		std::cout << "Before color is " << newColor.c << " " << newColor.m << " " << newColor.y << " " << newColor.k << std::endl; // this is wrong
		newColor = newColor + allColors[i]->GetTile()->GetColor();
		std::cout << "After color is " << newColor.c << " " << newColor.m << " " << newColor.y << " " << newColor.k << std::endl; // this is wrong
	}
	newColor = newColor / (float) allColors.size();
	return newColor;
}

void PaintBlobCombiner::Update()
{
	if (m_output == NULL)
	{
		if (getObjectsInInv().size() == m_NumToCombine)
		{
			std::shared_ptr<GameObject> o1 = GetObjectAt(0);
			
			std::vector<PaintBlob*> allColors;
			if (PaintBlob* p1 = dynamic_cast<PaintBlob*>(o1.get())) {
				for (unsigned int i = 0; i < getObjectsInInv().size(); i++)
				{
					if (PaintBlob* p2 = dynamic_cast<PaintBlob*>(GetObjectAt(i).get())) {
						allColors.push_back(p2);
					}
				}
				p1->SetColor(CombineColors(allColors));
			}	

			m_output = o1;
			getObjectsInInv().clear();
			getObjectNumMoves().clear();
		}
	}
	if (m_output != NULL && getNextObject()->AllowNewItem())
	{
		if (GetDirection() == DirectionUp) m_output->SetPos(GetPos() + Vec3{ .5,1,0 });
		if (GetDirection() == DirectionDown) m_output->SetPos(GetPos() + Vec3{ .5,-1,0 });
		if (GetDirection() == DirectionLeft) m_output->SetPos(GetPos() + Vec3{ -1,.5,0 });
		if (GetDirection() == DirectionRight) m_output->SetPos(GetPos() + Vec3{ 1,.5,0 });
		getNextObject()->AddObject(m_output);
		m_output = NULL;
	}
}

void PaintBlobCombiner::Render()
{
}

bool PaintBlobCombiner::AllowNewItem(bool StartAtHalf)
{
	if (getObjectsInInv().size() < (unsigned int) m_NumToCombine)
		return true;
	return false;
}
