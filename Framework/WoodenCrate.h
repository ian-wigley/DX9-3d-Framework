#pragma once
#include "SceneNode.h"
#include "SceneGraph.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include "TerrainNode.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"

class TerrainNode;

class WoodenCrate :
	public SceneNode
{
public:
	WoodenCrate(void);
	~WoodenCrate(void);
	WoodenCrate(wstring name, Framework *_frame, float x, float z,	FrameWorkResourceManager* _frameResourcesManager);
	HRESULT Render(void);
	void Shutdown(void);

private:
	HRESULT InitialiseCrate(void);
	D3DMATERIAL9 _material;
	TerrainNode* _terrainNode;
	FrameWorkResourceManager* _frameWorkResourcesManager;
	BoundingShape* _boundingShape;
	BoundingSphere* _boundingSphere;
	BoundingBox* _boundingBox;
	SceneGraph* _sGraph;
};
