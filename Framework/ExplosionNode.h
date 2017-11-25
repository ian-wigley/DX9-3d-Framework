#pragma once
#include "scenenode.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include "TerrainNode.h"

class ExplosionNode :
	public SceneNode
{
public:
	ExplosionNode(void);
	ExplosionNode::ExplosionNode(wstring name, Framework* frame, D3DXVECTOR3 position, FrameWorkResourceManager* frameResourcesManager);
	~ExplosionNode(void);
	HRESULT Render(void);
	void Update(void);
	bool DeleteMe(void);

	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;      // The untransformed, 3D position for the vertex
		D3DXVECTOR3 normal; // The normal for the vertex
		FLOAT u, v;			// Texture coordinates
	};

	// Custom FVF, which describes our custom vertex structure
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

	void Shutdown(void);

private:

	float _u1;
	float _u2;

	HRESULT InitialiseExplosion(void);

	D3DMATERIAL9 _material;
	LPDIRECT3DVERTEXBUFFER9 g_pVB;

	Framework* _frame;
	FrameWorkResourceManager* _frameWorkResourcesManager;
};

