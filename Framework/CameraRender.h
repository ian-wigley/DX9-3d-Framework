#pragma once
#include "cameranode.h"

class TerrainNode;
class Tank;

class CameraRender :
	public CameraNode
{
public:
	CameraRender(void);
	~CameraRender(void);
	CameraRender(LPDIRECT3DDEVICE9 pd3dDevice , TerrainNode* terrainNode, Tank* tank);

	HRESULT Render(void);

	void Update(void);

	void MoveForward(float amountOfMovement);
	void MoveRight(float amountOfMovement);
	void MoveUp(float amountOfMovement);

	void Roll(float amountOfMovement);
	void Yaw(float amountOfMovement);
	void Pitch(float amountOfMovement);

	void SetForward(void);

	D3DXVECTOR3 GetPosition(void)const;
	D3DXVECTOR3 GetLookAt(void)const;
	D3DXVECTOR3 GetRight(void)const;
	void SetPosition(D3DXVECTOR3 _gamepadPosition);

	void SetLookAt(const D3DXVECTOR3 &position, float angle);


	D3DXMATRIX GetViewMatrix(void)const;
	void SetCameraType(bool cameraType);
	bool GetCameraType(void);

private:
	D3DXVECTOR3 targetPosition;

	float _offsetHeight;
	float _cameraSpeed;

	float _fogStart;
	float _fogEnd;
	
	bool _thirdPerson;

	LPDIRECT3DDEVICE9 _pd3dDevice;
	D3DXMATRIX _projectionMatrix;

	CameraNode*_NodeToRender;
	TerrainNode* _terrain;
	Tank* _tank;
};