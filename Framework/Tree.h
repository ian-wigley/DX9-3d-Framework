#pragma once
#include "scenenode.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include "TerrainNode.h"

class TerrainNode;

class Tree :
	public SceneNode
{
public:
	Tree(void);
	Tree(wstring name, Framework* frame, float x, float z, FrameWorkResourceManager* _frameResourcesManager);
	~Tree(void);
	HRESULT Render(void);

private:
	void Shutdown(void);
	Framework* _frame;
	FrameWorkResourceManager* _frameWorkResourcesManager;
	TerrainNode* _terrainNode;
};