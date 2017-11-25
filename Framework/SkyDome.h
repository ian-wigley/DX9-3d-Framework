#pragma once
#include "SceneNode.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include "CameraRender.h"

class SkyDome :
	public SceneNode
{
public:
	SkyDome(void);
	~SkyDome(void);
	SkyDome(wstring name, Framework* frame, FrameWorkResourceManager* _frameResourcesManager);
	HRESULT Render(void);

private:
	void Shutdown(void);
	HRESULT InitialiseDome(void);
	void LoadDome(void);
	
	D3DXMATRIX _saveView;

	CameraRender* _renderCamera;
	Framework* _frame;
	FrameWorkResourceManager* _frameWorkResourcesManager;
};

