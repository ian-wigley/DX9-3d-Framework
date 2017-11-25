//-----------------------------------------------------------------------------
// File: SpotLight.cpp
// Description : This Class managers the SpotLight
// Author : Ian Wigley

//-----------------------------------------------------------------------------


#include "SpotLight.h"


SpotLight::SpotLight(void)
{
	// Setup a point light
	D3DLIGHT9 pointLight;
	ZeroMemory(&pointLight, sizeof(pointLight));
	pointLight.Type = D3DLIGHT_POINT;
	pointLight.Diffuse.r = 0.43f;
	pointLight.Diffuse.g = 0.57f;
	pointLight.Diffuse.b = 0.74f;
	pointLight.Diffuse.a = 1.0f;

	pointLight.Specular.r = 1.0f;
	pointLight.Specular.g = 1.0f;
	pointLight.Specular.b = 1.0f;
	pointLight.Specular.a = 1.0f;

	pointLight.Position = D3DXVECTOR3(-2.0f, 0.0f, -1.0f);
	pointLight.Attenuation0 = 0.0f;
	pointLight.Attenuation1 = 1.0f;
	pointLight.Attenuation2 = 0.0f;
	pointLight.Falloff = 1.0f;
	// Make sure you set the range (otherwise it will have a default value 0f 0 and you won't see a light
	pointLight.Range = 1000;
	_pd3dDevice->SetLight(0, &pointLight);
	_pd3dDevice->LightEnable(0, true);
}


SpotLight::~SpotLight(void)
{
}
