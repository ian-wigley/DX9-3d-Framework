#pragma once
#include "SceneGraph.h"
#include "CameraRender.h"

#include <XInput.h>
#pragma comment(lib, "XInput.lib")

class Tank;

class GamePadController
{
public:
	GamePadController(void);
	~GamePadController(void);
	void ProcessGameController();
	void SetCamera(CameraRender* _renderCam);
	void SetTank(Tank* tank);

private:
	XINPUT_STATE _controllerState;
	DWORD		 _lastPacketNumber;
	bool		 _firstTime;
	bool		 _switch;
	bool		 _thirdPerson;

	// These two values are used to avoid having to calculate square roots (which are very time consuming)
	// when we are checking if the movement of the left or right thumb stick is in the dead zone
	DWORD		_leftThumbDeadZoneSquared;
	DWORD		_rightThumbDeadZoneSquared;

	CameraRender* _renderCamera;
	Tank* _tank;
};

