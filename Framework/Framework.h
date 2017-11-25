#include "SceneGraph.h"
#include <Windows.h>
#include <string>
#include "GamePadController.h"
#include <XInput.h>
#include "CameraRender.h"
#include "CameraNode.h"


#include <d3dx9.h>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

class Tank;
class SkyDome;
class FrameWorkResourceManager;


using namespace std;

#pragma once
class Framework
{
public:
	Framework(void);
	Framework(HINSTANCE hInstance);

	~Framework(void);
	LPDIRECT3DDEVICE9 GetDirect3dDevice(void); 
	SceneGraph * GetSceneGraph(void); 


	HWND GetHandle(void);

	// Framework methods.  Derived client class overrides these methods to  
	// implement specific application requirements. 
 
	virtual bool Initialise(HINSTANCE hInstance);
	virtual void Shutdown(void);
	virtual int Run(void); 
	virtual bool CheckDeviceCapabilities(void); 
	virtual void OnLostDevice(void); 
	virtual void OnResetDevice(void); 
	virtual void Update(void);
	virtual void Render(void); 

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); 
 	HRESULT InitD3D( HWND hWnd );

	static LRESULT CALLBACK windPROC(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void SetCamera(CameraRender* _renderCam);
	CameraRender* GetCamera(void)const;
	void SetTank(Tank* tank);
	void SetTerrain(TerrainNode* terrainNode);
	TerrainNode* GetTerrain(void)const;
	void GetController(void)const;

	void SetObjects(CameraRender* camRender, Tank* tank, SkyDome* skyDome);
	void SetObjects(CameraRender* camRender, Tank* tank, SkyDome* skyDome, FrameWorkResourceManager* frameResourcesManager);

protected:
	#define SAFE_DELETE(x) { delete x; x = NULL; }
	#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }


private:

	int _oldMouseX;
	int _oldMouseY;
	int _mouseX;
	int _mouseY;

	MSG msg;
	HWND _hWnd;
	SceneGraph* _scene;

	void SetupMatrices(void);

	// Used to create the D3DDevice
	LPDIRECT3D9         _pD3D;
	// Our rendering device
	LPDIRECT3DDEVICE9   _pd3dDevice;
	LPD3DXMESH			_pMesh;

	GamePadController* _controller;
	CameraRender* _renderCamera;
	CameraNode*_cameraNode;
	SkyDome* _skyDome;
	Tank* _tank;
	TerrainNode* _terrain;
};