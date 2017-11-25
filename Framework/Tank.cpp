//-----------------------------------------------------------------------------
// File: Tank.cpp
// Description : This Class managers the Tank
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "Tank.h"

//-----------------------------------------------------------------------------
// Name: Constructor
// Desc: Creates the Tank
//-----------------------------------------------------------------------------
Tank::Tank(void)
{
}

//-----------------------------------------------------------------------------
// Name: Constructor
// Desc: Creates the Tank
//-----------------------------------------------------------------------------
Tank::Tank(wstring name, Framework* frame, float x, float z, FrameWorkResourceManager* frameResourcesManager)
{
	D3DXMatrixIdentity(&_worldMatrix);
	_thirdPerson = false;
	_fire = false;
	_scale = 0.5f;
	_angle = 0;
	_x = x;
	_y = 0;
	_z = z;
	_terrainExtent = 1260.0f;
	_name = name;
	_frame = frame;
	_texture = NULL;
	_collsion = false;
	_delete = false;
	_pd3dDevice = _frame->GetDirect3dDevice();
	_sGraph =_frame->GetSceneGraph();
	_renderCam = _frame->GetCamera();

	_bulletCount = 0;

 	_cameraSpeed = 10.0f;
	_yaw = 0.0f;					// Degrees of yaw (in radians)
	_pitch = 0.0f;					// Degrees of pitch (in radians)
	_roll = 0.0f;

	_terrainNode = _frame->GetTerrain();

	// Get the height of the terrain at x, z
	_y = _terrainNode->GetHeight(_x, _z);

	_position = D3DXVECTOR3(_x, _y, _z);

	_frameWorkResourcesManager = frameResourcesManager;
	_mesh = _frameWorkResourcesManager->GetMesh(L"Tiger.x", false);
	_texture = _frameWorkResourcesManager->GetTexture(L"Tiger.x");
	_material = _frameWorkResourcesManager->GetMaterial(L"Tiger.x");

	_sGraph->AddNode(_boundingBox = new BoundingBox(_mesh, _pd3dDevice, _x,_y,_z, L"boundingBox", _scale),L"Parent");

	// Get the 4 vectors
	_tankFrontLeftPos = _boundingBox->GetFrontLeftVector();
	_tankFrontMidPos = _boundingBox->GetFrontMidVector();
	_tankFrontRightPos = _boundingBox->GetFrontRightVector();
	_tankRearMidPos = _boundingBox->GetRearMidVector();

	// Calculate the Hypotenuse values
	_xHypotenuse =  _tankFrontLeftPos.x - _tankFrontRightPos.x ;
	_zHypotenuse =  _tankRearMidPos.z - _tankFrontMidPos.z;

	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 22;
    fontDesc.Width           = 0;
    fontDesc.Weight          = FW_BOLD;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	D3DXCreateFontIndirect(_pd3dDevice, &fontDesc, &mFont);
}

//-----------------------------------------------------------------------------
// Name: ~Tank()
// Desc: Tank Class Destructor
//-----------------------------------------------------------------------------
Tank::~Tank(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Deletes the objects used in the Class
//-----------------------------------------------------------------------------
void Tank::Shutdown(void)
{
	if(_texture)
	{
		SAFE_RELEASE(_texture);
	}
	if(_mesh)
	{
		SAFE_DELETE(_mesh);
	}
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
HRESULT Tank::Render(void)
{
	_worldMatrix._11 = _right.x;
	_worldMatrix._21 = _right.y;
	_worldMatrix._31 = _right.z;

	_worldMatrix._12 = _up.x;
	_worldMatrix._22 = _up.y;
	_worldMatrix._32 = _up.z;

	_worldMatrix._13 = _lookAt.x;
	_worldMatrix._23 = _lookAt.y;
	_worldMatrix._33 = _lookAt.z;

	_worldMatrix._41 = -D3DXVec3Dot(&_position, &_right);
	_worldMatrix._42 = -D3DXVec3Dot(&_position, &_up);
	_worldMatrix._43 = -D3DXVec3Dot(&_position, &_lookAt);

    D3DXMatrixRotationX(&_rotationMatrixX, _roll);
	D3DXMatrixRotationY(&_rotationMatrixY, _angle);
	D3DXMatrixRotationZ(&_rotationMatrixZ, _pitch);

	D3DXMatrixTranslation(&_translationMatrix, _position.x, _position.y, _position.z);

	D3DXMatrixScaling(&_scalingingMatrix,_scale,_scale,_scale);

	_worldMatrix = _rotationMatrixY * _rotationMatrixY * _rotationMatrixZ * _scalingingMatrix *_translationMatrix;
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    _pd3dDevice->SetTexture( 0, _texture );

	// Draw the mesh subset
	_mesh->DrawSubset( 0 );


	_boundingBox->UpdateBoundingShape(_worldMatrix);


	// If Camera is set to 3rd person then update the posiotn and angle
	if (_thirdPerson)
	{
		_renderCam->SetLookAt(_position, _angle);
	}

	TextDisplay();

	_position.y = _terrainNode->GetHeight(_tankFrontLeftPos.x + _position.x, _tankFrontLeftPos.z  + _position.z);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Updates the Tank
//-----------------------------------------------------------------------------
void Tank::Update(void)
{
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	// Rotate lookat and right vectors around the up vector
	D3DXMatrixRotationAxis(&yawMatrix, &_up, _yaw);
	D3DXVec3TransformCoord(&_lookAt, &_lookAt, &yawMatrix);
	D3DXVec3TransformCoord(&_right, &_right, &yawMatrix);

	// Rotate lookat and up vectors around the right vector
	D3DXMatrixRotationAxis(&pitchMatrix, &_right, _pitch);
	D3DXVec3TransformCoord(&_lookAt, &_lookAt, &pitchMatrix);
	D3DXVec3TransformCoord(&_up, &_up, &pitchMatrix);

	// Rotate right and up vectors around the lookat vector
	D3DXMatrixRotationAxis(&rollMatrix, &_lookAt, _roll);
	D3DXVec3TransformCoord(&_right, &_right, &rollMatrix);
	D3DXVec3TransformCoord(&_up, &_up, &rollMatrix);

	// Calculate the opposite length for s=o/h required for the roll calculation
	_xOpposite = _terrainNode->GetHeight(_tankFrontLeftPos.x + _position.x, _tankFrontLeftPos.z + _position.z) - 
				_terrainNode->GetHeight(_tankFrontRightPos.x + _position.x, _tankFrontRightPos.z + _position.z);

	CalculatePitch(_zOpposite, _zHypotenuse);


	// Calculate the opposite length for s=o/h required for the pitch calculation
	_zOpposite = _terrainNode->GetHeight(_tankFrontLeftPos.x + _position.x, _tankFrontLeftPos.z + _position.z) -
				_terrainNode->GetHeight(_tankFrontRightPos.x + _position.x, _tankFrontRightPos.z + _position.z);

	CalculateRoll(_xOpposite, _xHypotenuse);
}

//-----------------------------------------------------------------------------
// Name: FireBullet()
// Desc: Creates a new Bullet
//-----------------------------------------------------------------------------
void Tank::FireBullet(void)
{
	_bulletCount++;
	wstringstream streamVal;
	streamVal << _bulletCount;
	wstring _bulletName = L"BulletNode"+ streamVal.str();
	_sGraph->AddNode(_bullet = new BulletNode(_bulletName, _frame, _position, _angle, _frameWorkResourcesManager),L"Parent");
}


//-----------------------------------------------------------------------------
// Name: MoveForward()
// Desc: Moves the tank forward
//-----------------------------------------------------------------------------
void Tank::MoveForward(float amountOfMovement)
{

	if (_position.z < _terrainExtent && _position.z > _terrainExtent * -1)
	{
		_position += _lookAt * amountOfMovement * _cameraSpeed;
	}

	// Stop the tank from falling off the terrain
	if (_position.z > _terrainExtent)
	{
		_position.z = 1259.0f;
	}
	else if (_position.z < _terrainExtent * -1)
	{
		_position.z = -1259.0f;
	}

	if (_position.x > _terrainExtent)
	{
		_position.x = 1259.0f;
	}
	else if (_position.x < _terrainExtent * -1)
	{
		_position.x = -1259.0f;
	}
}

//-----------------------------------------------------------------------------
// Name: Yaw()
// Desc: Calculates the Yaw
//-----------------------------------------------------------------------------
void Tank::Yaw(float amountOfMovement)
{
	_yaw += amountOfMovement;
	_angle += amountOfMovement;
	if (_angle > 3.141 || _angle < -3.141)
	{
		_angle = 0;
		_yaw = 0;
	}
}

//-----------------------------------------------------------------------------
// Name: CalculateRoll()
// Desc: Calculates the Roll
//-----------------------------------------------------------------------------
void Tank::CalculateRoll(float xOpposite, float xHypotenuse)
{
	_roll = sin(xOpposite/xHypotenuse);
}

//-----------------------------------------------------------------------------
// Name: CalculatePitch()
// Desc: Calculates the Pitch
//-----------------------------------------------------------------------------
void Tank::CalculatePitch(float zOpposite, float zHypotenuse)
{
	_pitch = sin(zOpposite/zHypotenuse);
}

//-----------------------------------------------------------------------------
// Name: SetPosition()
// Desc: Sets the Position
//-----------------------------------------------------------------------------
void Tank::SetPosition(D3DXVECTOR3 distance)
{
	_position += distance;
}

//-----------------------------------------------------------------------------
// Name: SetCameraType()
// Desc: Changes between 1st and 3rd person camera
//-----------------------------------------------------------------------------
void Tank::SetCameraType(bool cameraType)
{
	_thirdPerson = cameraType;
}