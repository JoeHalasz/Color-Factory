#include "PaintBlobCombiner.h"

PaintBlobCombiner::PaintBlobCombiner(Vec3 pos, int size, Direction direction, bool isBasePart, TileType type)
{
	SetPos(pos);
	SetSize(size);
	SetDirection(direction);
	SetIsBasePart(isBasePart);
	WorldTile tile = WorldTile(type);
	SetTile(&tile);
}

void PaintBlobCombiner::Update()
{
	if (m_output == NULL)
	{
		if (getObjectsInInv().size() == 2)
		{
			std::shared_ptr<GameObject> o1 = GetObjectAt(0);
			std::shared_ptr<GameObject> o2 = GetObjectAt(1);
			
			if (PaintBlob* p1 = dynamic_cast<PaintBlob*>(o1.get()))
				if (PaintBlob* p2 = dynamic_cast<PaintBlob*>(o2.get()))
					p1->CombineColor(p2);

			m_output = o1;
			getObjectsInInv().clear();
			getObjectNumMoves().clear();
		}
	}
	if (m_output != NULL && getNextObject()->AllowNewItem())
	{
		if (GetDirection() == DirectionUp) m_output->SetPos(m_output->GetPos() + Vec3{ 0,1,0 });
		if (GetDirection() == DirectionDown) m_output->SetPos(m_output->GetPos() + Vec3{ 0,-1,0 });
		if (GetDirection() == DirectionLeft) m_output->SetPos(m_output->GetPos() + Vec3{ -1,0,0 });
		if (GetDirection() == DirectionRight) m_output->SetPos(m_output->GetPos() + Vec3{ 1,0,0 });
		getNextObject()->AddObject(m_output);
		m_output = NULL;
	}
}

void PaintBlobCombiner::Render()
{
}

bool PaintBlobCombiner::AllowNewItem(bool StartAtHalf)
{
	std::cout << getObjectsInInv().size() << std::endl;
	if (getObjectsInInv().size() < 2)
		return true;
	return false;
}
