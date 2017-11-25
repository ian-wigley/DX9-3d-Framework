//-----------------------------------------------------------------------------
// File: ViewMatrixCode.cpp
// Description : This Class managers the ViewMatrix
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "ViewMatrixCode.h"


ViewMatrixCode::ViewMatrixCode(void)
{
	// This is the basic code needed to setup a view matrix
	// This can be incorporated into your code appropriately
	// Reset vectors
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);;
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	// Setup matrices to rotate around axes
	D3DXMatrixRotationAxis(&yawMatrix, &_up, yaw);
	D3DXMatrixRotationAxis(&pitchMatrix, &_right, pitch);
	D3DXMatrixRotationAxis(&rollMatrix, &_lookAt, roll);

	// Rotate lookat and right vectors around the up vector
	D3DXVec3TransformCoord(&_lookAt, &_lookAt, &yawMatrix);
	D3DXVec3TransformCoord(&_right, &_right, &yawMatrix);

	// Rotate lookat and up vectors around the right vector
	D3DXVec3TransformCoord(&_lookAt, &_lookAt, &pitchMatrix);
	D3DXVec3TransformCoord(&_up, &_up, &pitchMatrix);

	// Rotate right and up vectors around the lookat vector
	D3DXVec3TransformCoord(&_right, &_right, &rollMatrix);
	D3DXVec3TransformCoord(&_up, &_up, &rollMatrix);

	// Now setup view matrix
	D3DXMatrixIdentity (&_viewMatrix);
	_viewMatrix._11 = _right.x;
	_viewMatrix._21 = _right.y;
	_viewMatrix._31 = _right.z;

	_viewMatrix._12 = _up.x;
	_viewMatrix._22 = _up.y;
	_viewMatrix._32 = _up.z;

	_viewMatrix._13 = _lookAt.x;
	_viewMatrix._23 = _lookAt.y;
	_viewMatrix._33 = _lookAt.z;

//	D3DXVECTOR3 position;

	_viewMatrix._41 = -D3DXVec3Dot(&position, &_right);
	_viewMatrix._42 = -D3DXVec3Dot(&position, &_up);
	_viewMatrix._43 = -D3DXVec3Dot(&position, &_lookAt);
}


ViewMatrixCode::~ViewMatrixCode(void)
{
}


