//-----------------------------------------------------------------------------
// File: DirectionalLight.cpp
// Description : This Class managers the DirectionalLight
// Author : Ian Wigley

//-----------------------------------------------------------------------------

#include "DirectionalLight.h"

//-----------------------------------------------------------------------------
// Name: DirectionalLight()
// Desc: DirectionalLight Class Constructor
//-----------------------------------------------------------------------------
DirectionalLight::DirectionalLight(void)
{
}

//-----------------------------------------------------------------------------
// Name: DirectionalLight()
// Desc: BoundingBox Class Constructor
//-----------------------------------------------------------------------------
DirectionalLight::DirectionalLight(wstring name, Framework *_frame)
{
	_name = name;
	_pd3dDevice = _frame->GetDirect3dDevice();
	SetuplLight();
}

//-----------------------------------------------------------------------------
// Name: ~DirectionalLight()
// Desc: DirectionalLight Class Destructor
//-----------------------------------------------------------------------------
DirectionalLight::~DirectionalLight(void)
{
}

//-----------------------------------------------------------------------------
// Name: SetuplLight()
// Desc: Creates the Light
//-----------------------------------------------------------------------------
void DirectionalLight::SetuplLight(void)
{
	// Setup a directional light
	D3DLIGHT9 directionalLight;
	ZeroMemory(&directionalLight, sizeof(directionalLight));
	directionalLight.Type = D3DLIGHT_DIRECTIONAL;
	directionalLight.Diffuse.r = 0.90f;
	directionalLight.Diffuse.g = 0.69f;
	directionalLight.Diffuse.b = 0.0f;
	directionalLight.Diffuse.a = 1.0f;

	directionalLight.Specular.r = 1.0f;
	directionalLight.Specular.g = 1.0f;
	directionalLight.Specular.b = 1.0f;
	directionalLight.Specular.a = 1.0f;

	directionalLight.Direction = D3DXVECTOR3(10.0f, 0.0f, 1.0f);
	_pd3dDevice->SetLight(0, &directionalLight);
	_pd3dDevice->LightEnable(0, true);
}
