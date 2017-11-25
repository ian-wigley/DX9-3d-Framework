#pragma once
#include "BoundingShape.h"
#include "BoundingBox.h"
#include "scenenode.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include "TerrainNode.h"


class Castle :
	public SceneNode
{
public:
	Castle(void);
	Castle(wstring name, Framework* frame, float x, float z, FrameWorkResourceManager* _frameResourcesManager);
	~Castle(void);
	HRESULT Render(void);

private:
	void Shutdown(void);

	BoundingShape* _boundingShape;
	BoundingBox* _boundingBox;
	Framework* _frame;
	FrameWorkResourceManager* _frameWorkResourcesManager;
	SceneGraph* _sGraph;
	TerrainNode* _terrainNode;
};

