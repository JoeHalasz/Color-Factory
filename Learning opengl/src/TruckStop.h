#pragma once
#include <TruckNode.h>
#include <PaintBlob.h>


class TruckStop
{
private:
	std::shared_ptr<TruckNode> thisNode;
	PaintBlob m_PaintBlobType = PaintBlob({ 0,0,0 }, 0, { 0,0,0,0 }, DirectionUp);
	int m_NumPaintBlobs = 0;
	int m_MaxPaintBlobs = 10;

public:
	TruckStop() : thisNode(std::make_shared<TruckNode>()) {}
	TruckStop(std::shared_ptr<TruckNode> node) : thisNode(node) {}

	std::shared_ptr<TruckNode> GetNode() const { return thisNode; }

	inline void SetPaintBlobType(PaintBlob paintBlob) { m_PaintBlobType = paintBlob; }
	inline PaintBlob GetPaintBlobType() const { return m_PaintBlobType; }
	inline bool AddPaintBlob() { if (m_NumPaintBlobs < m_MaxPaintBlobs) { m_NumPaintBlobs++; return true; } return false;}
	inline bool RemovePaintBlob() {if (m_NumPaintBlobs > 0) { m_NumPaintBlobs--; return true; } return false;}
};