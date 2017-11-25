//-----------------------------------------------------------------------------
// File: Tree.cpp
// Description : This Class managers the Tree
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "Castle.h"

//-----------------------------------------------------------------------------
// Name: Castle()
// Desc: Castle Class Constructor
//-----------------------------------------------------------------------------
Castle::Castle(void)
{
}

//-----------------------------------------------------------------------------
// Name: ~Castle()
// Desc: Castle Class Destructor
//-----------------------------------------------------------------------------
Castle::~Castle(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Free memory & resources
//-----------------------------------------------------------------------------
void Castle::Shutdown(void)
{
	if(_texture)
	{
		SAFE_RELEASE(_texture);
	}
	if(_mesh)
	{
		SAFE_DELETE(_mesh);
	}
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}
}

//-----------------------------------------------------------------------------
// Name: Castle()
// Desc: Castle Class Constructor
//-----------------------------------------------------------------------------
Castle::Castle(wstring name, Framework* frame, float x, float z, FrameWorkResourceManager* _frameResourcesManager)
{
	_x = x;
	_y = 0;
	_z = z;
	_name = name;
	_frame = frame;
	_texture = NULL;
	_scale = 1.0f;
	_collsion = false;
	_delete = false;
	_pd3dDevice = _frame->GetDirect3dDevice();
	_sGraph =_frame->GetSceneGraph();
	_terrainNode = _frame->GetTerrain();

	_frameWorkResourcesManager = _frameResourcesManager;
	_mesh = _frameWorkResourcesManager->GetMesh(L"Castle.x", false);
	_material = _frameWorkResourcesManager->GetMaterial(L"Castle.x");
	_texture = _frameWorkResourcesManager->GetTexture(L"Castle.x");
	_numberOfMaterials = _frameWorkResourcesManager->GetNumberOfMatrials();

	_y = _terrainNode->GetHeight(_x, _z) - 5.0f;

	_sGraph->AddNode(_boundingBox = new BoundingBox(_mesh, _pd3dDevice, _x,_y,_z, L"boundingBox", _scale),L"Parent");
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Render the Castle
//-----------------------------------------------------------------------------
HRESULT Castle::Render(void)
{
	D3DXMatrixTranslation( &_worldMatrix, _x, _y, _z);
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// Set texture to use
    _pd3dDevice->SetTexture(0, _texture);
    _pd3dDevice->SetTexture(1, _texture);
	_pd3dDevice->SetMaterial(_material);

	for (DWORD i=0; i < _numberOfMaterials; i++)
	{
		// Draw the mesh subsets
		_mesh->DrawSubset(i);
	}
	return S_OK;
}

