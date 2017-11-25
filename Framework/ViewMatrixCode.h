#include "SceneNode.h"
#pragma once
class ViewMatrixCode :
	public SceneNode
{

public:
	ViewMatrixCode(void);
	~ViewMatrixCode(void);

private:
	float yaw;						// Degrees of yaw (in radians)
	float pitch;					// Degrees of pitch (in radians)
	float roll;						// Degrees of roll (in radians)
	D3DXVECTOR3 position;			// Position of camera in world space

	D3DXMATRIXA16 yawMatrix;
	D3DXMATRIXA16 pitchMatrix;
	D3DXMATRIXA16 rollMatrix;

	D3DXVECTOR3 _up;
	D3DXVECTOR3 _lookAt;
	D3DXVECTOR3 _right;
	D3DXMATRIX  _viewMatrix;
};

