//-----------------------------------------------------------------------------
// File: SkyDome.cpp
// Description : This Class managers the SkyDome
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "SkyDome.h"

//-----------------------------------------------------------------------------
// Name: SkyDome()
// Desc: SkyDome Class Constructor
//-----------------------------------------------------------------------------
SkyDome::SkyDome(void)
{
}

//-----------------------------------------------------------------------------
// Name: SkyDome()
// Desc: SkyDome Class Constructor
//-----------------------------------------------------------------------------
SkyDome::SkyDome(wstring name, Framework* frame, FrameWorkResourceManager* _frameResourcesManager)
{
	_delete = false;
	_frame = frame;
	_name = name;
	_pd3dDevice = _frame->GetDirect3dDevice();
	_position = D3DXVECTOR3(_x, _y, _z);
	_frameWorkResourcesManager = _frameResourcesManager;
	_mesh = _frameWorkResourcesManager->GetMesh(L"dome.x", false);
	_texture = _frameWorkResourcesManager->GetTexture(L"dome.x");
}

//-----------------------------------------------------------------------------
// Name: ~SkyDome()
// Desc: SkyDome Class Destructor
//-----------------------------------------------------------------------------
SkyDome::~SkyDome(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Free memory & resources
//-----------------------------------------------------------------------------
void SkyDome::Shutdown(void)
{
	if(_texture)
	{
		SAFE_RELEASE(_texture);
	}
	if(_mesh)
	{
		SAFE_RELEASE(_mesh);
	}
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
HRESULT SkyDome::Render(void)
{
	_renderCamera = _frame->GetCamera();

	//Get the view Matrix from the camera
	_viewMatrix = _renderCamera->GetViewMatrix();

	//Store the view matrix in SaveView
	_pd3dDevice->GetTransform(D3DTS_VIEW, &_saveView);

	_viewMatrix._41 = 0.0f;
	_viewMatrix._42 = 0.0f;
	_viewMatrix._43 = 0.0f;

	_pd3dDevice->SetTransform(D3DTS_VIEW, &_viewMatrix);

	D3DXMatrixIdentity(&_worldMatrix);

	D3DXMatrixTranslation(&_translationMatrix, 0.0f, -200.0f, 100.0f);
	D3DXMatrixScaling(&_scalingingMatrix, 2000.0f, 2000.0f, 2000.0f);
	D3DXMatrixRotationY(&_rotationMatrixY, DEGTORAD(90));
	_worldMatrix = _rotationMatrixY * _scalingingMatrix *_translationMatrix;

	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);
	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

  	// Set texture to render
    _pd3dDevice->SetTexture( 0, _texture );

	// Draw the mesh subset
	_mesh->DrawSubset( 0 );
	
	// Restore the view Matrix back to it's original state
	_pd3dDevice->SetTransform(D3DTS_VIEW,	&_saveView);	
	return S_OK;
}