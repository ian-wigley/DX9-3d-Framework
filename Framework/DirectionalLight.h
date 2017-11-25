#pragma once
#include "light.h"
#include "Framework.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight(void);
	~DirectionalLight(void);
	DirectionalLight(wstring name, Framework *_frame);

private:
	void SetuplLight();
	LPDIRECT3DDEVICE9   _pd3dDevice;
};