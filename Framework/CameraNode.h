#pragma once
#include "scenenode.h"

class CameraNode :
	public SceneNode
{
public:
	CameraNode(void);
	~CameraNode(void);
	CameraNode(wstring name);
	void Update(void);

protected:
	float amountOfMovement;
	float yaw;						// Degrees of yaw (in radians)
	float pitch;					// Degrees of pitch (in radians)
	float roll;						// Degrees of roll (in radians)
};