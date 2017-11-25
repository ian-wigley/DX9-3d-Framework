#pragma once
#include "scenenode.h"
#include "SceneGraph.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include "TerrainNode.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include "CameraRender.h"
#include "ExplosionNode.h"


class BulletNode :
	public SceneNode
{
public:
	BulletNode(void);
	BulletNode::BulletNode(wstring name, Framework* frame, D3DXVECTOR3 position, float angle, FrameWorkResourceManager* frameResourcesManager);
	~BulletNode(void);
	HRESULT Render(void);
	void Update(void);
	bool DeleteMe(void);
	void ExternalDeleteMe(bool deleteMe);

private:
	void Shutdown(void);
	void InitialiseBullet(void);

	float _maxDistance;
	float _startDistance;

	D3DMATERIAL9 _materials;

	BoundingShape* _boundingShape;
	BoundingSphere* _boundingSphere;

	Framework* _frame;
	FrameWorkResourceManager* _frameWorkResourcesManager;
	SceneGraph* _sGraph;
	TerrainNode* _terrainNode;
	ExplosionNode* _explosion;
};

