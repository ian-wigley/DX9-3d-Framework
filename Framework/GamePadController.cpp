//-----------------------------------------------------------------------------
// File: GamePadController.cpp
// Description : This Class managers the Game Pad Controller
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "GamePadController.h"
#include "Tank.h"

GamePadController::	GamePadController(void)
{
	_switch = true;
	_firstTime = true;
	_leftThumbDeadZoneSquared = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	_rightThumbDeadZoneSquared = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE * XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

GamePadController::~GamePadController(void)
{
}

void GamePadController::ProcessGameController()
{
	_thirdPerson = _renderCamera->GetCameraType();

	float _amountOfMovement = 0.5f;
	float _rotationAmount = 0.015f;

	DWORD magnitudeSquared;

	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	// Get the state of the controller
    if (XInputGetState(0, &_controllerState) != ERROR_SUCCESS)
	{
		// Controller is not connected or is lost
 		return;
	}
	// Uncomment out this if statement if you only want to test the controller if it has
	// changed since the last time it was tested
	//if (!_firstTime && _controllerState.dwPacketNumber != _lastPacketNumber)
	//{
	//	// No change in controller states
	//}
	_firstTime = false;

	// Deal with the left thumb stick
	SHORT thumbLX = _controllerState.Gamepad.sThumbLX;
	SHORT thumbLY = _controllerState.Gamepad.sThumbLY;

	// Determine how far the controller is pushed
	magnitudeSquared = thumbLX * thumbLX + thumbLY * thumbLY;

	// check if the controller is inside a circular dead zone. We do it this way to avoid having to
	// take the square root of the magnitude above.
	if (magnitudeSquared <= _leftThumbDeadZoneSquared)
	{
		thumbLX = 0;
		thumbLY = 0;
	}

	// Deal with the right thumb stick
	SHORT thumbRX = _controllerState.Gamepad.sThumbRX;
	SHORT thumbRY = _controllerState.Gamepad.sThumbRY;

	// Determine how far the controller is pushed
	magnitudeSquared = thumbRX * thumbRX + thumbRY * thumbRY;

	//check if the controller is inside a circular dead zone
	if (magnitudeSquared <= _rightThumbDeadZoneSquared)
	{
		thumbRX = 0;
		thumbRY = 0;
	}

	// Check left and right triggers
	BYTE leftTrigger = _controllerState.Gamepad.bLeftTrigger;
	if (leftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		leftTrigger = 0;
	}

	BYTE rightTrigger = _controllerState.Gamepad.bRightTrigger;
	if (rightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		rightTrigger = 0;
	}

	// Test the different digital buttons
	WORD buttons = _controllerState.Gamepad.wButtons;
	if (buttons & XINPUT_GAMEPAD_DPAD_UP)
	{
		// Directional pad up pressed
		_renderCamera->SetPosition(_renderCamera->GetPosition() += _renderCamera->GetLookAt() * 0.5f);
	}
	if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		// Directional pad down pressed
		_renderCamera->SetPosition(_renderCamera->GetPosition() += -_renderCamera->GetLookAt() * 0.5f);
	}
	if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		// Directional pad left pressed
		_renderCamera->SetPosition(_renderCamera->GetPosition() += -_renderCamera->GetRight());
	}
	if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		// Directional pad right pressed
		_renderCamera->SetPosition(_renderCamera->GetPosition() += _renderCamera->GetRight());
	}

	//Test if the A button has been pressed (Fire)
	if (buttons & XINPUT_GAMEPAD_A)
	{
		if (!_switch)
		{
			_tank->FireBullet();
			_switch = true;
		}
	}

	// Test if the A button has been relesed - This stops the screen filling up with bullets 
	// when the A button is held down continously !
	if (!buttons)
	{
		if (_switch)
		{
			_switch = false;
		}
	}

	// Test the analogue left & right thumsticks

	// Left thumb pressed left
	if (thumbLX > 0)
	{
		if (!_thirdPerson)
		{
			_renderCamera->Yaw(_rotationAmount * 1);
		}
		else
		{
			_tank->Yaw(_rotationAmount * 1);
		}
	}

	// Left thumb pressed right
	if (thumbLX < 0)
	{
		if (!_thirdPerson)
		{
			_renderCamera->Yaw(_rotationAmount * -1);
		}
		else
		{
			_tank->Yaw(_rotationAmount * -1);
		}
	}


	// Right thumb pressed up 
	if (thumbRY > 0)
	{
		if (!_thirdPerson)
		{
			_renderCamera->MoveForward(_amountOfMovement);
		}
		else
		{
			_tank->MoveForward(_amountOfMovement);
		}
	}

	// Right thumb pressed down
	if (thumbRY < 0)
	{
		if (!_thirdPerson)
		{
			_renderCamera->MoveForward(_amountOfMovement * -1);
		}
		else
		{
			_tank->MoveForward(_amountOfMovement * -1);
		}
	}

	// Other button mask values that can be used are:
	//
	//  XINPUT_GAMEPAD_START, 
	//  XINPUT_GAMEPAD_BACK, 
	//  XINPUT_GAMEPAD_LEFT_THUMB, 
	//  XINPUT_GAMEPAD_RIGHT_THUMB, 
	//  XINPUT_GAMEPAD_LEFT_SHOULDER, 
	//  XINPUT_GAMEPAD_RIGHT_SHOULDER, 
	//  XINPUT_GAMEPAD_A, 
	//  XINPUT_GAMEPAD_B, 
	//  XINPUT_GAMEPAD_X, 
	//  XINPUT_GAMEPAD_Y 

}

void GamePadController::SetCamera(CameraRender* _renderCam)
{
	_renderCamera = _renderCam;
}

void GamePadController::SetTank(Tank* tank)
{
	_tank = tank;
}