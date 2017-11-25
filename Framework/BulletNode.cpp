//-----------------------------------------------------------------------------
// File: BulletNode.cpp
// Description : This Class managers the Bullets fired from the Tank
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "BulletNode.h"


BulletNode::BulletNode(void)
{
}

//-----------------------------------------------------------------------------
// Name: Constructor
// Desc: Creates the Tank
//-----------------------------------------------------------------------------
BulletNode::BulletNode(wstring name, Framework* frame, D3DXVECTOR3 position, float angle, FrameWorkResourceManager* frameResourcesManager)
{
	_position = position;
	_maxDistance = 200.0f + _position.z;
	_scale = 0.0f;
	_angle = angle;
	_x = _position.x;
	_y = _position.y + 9.0f;
	_z = _position.z;
	_name = name;
	_frame = frame;
	_texture = NULL;
	_collsion = false;
	_delete = false;
	_pd3dDevice = _frame->GetDirect3dDevice();
	_sGraph =_frame->GetSceneGraph();
	_position = D3DXVECTOR3(_x, _y, _z);
	_terrainNode = _frame->GetTerrain();
	_frameWorkResourcesManager = frameResourcesManager;
	_materials = _frameWorkResourcesManager->GetDefaultBlackMaterial();
	InitialiseBullet();
	_sGraph->AddNode(_boundingSphere = new BoundingSphere(_mesh, _pd3dDevice, _x,_y,_z, L"boundingSphere", _scale),L"Parent");
}


//-----------------------------------------------------------------------------
// Name: InitialiseBullet()
// Desc: Initialises the Bullet object
//-----------------------------------------------------------------------------
void BulletNode::InitialiseBullet(void)
{
    D3DXCreateSphere(_pd3dDevice, 1.0f , 16, 8,  &_mesh,  0);
}

//-----------------------------------------------------------------------------
// Name: ~BulletNode()
// Desc: BulletNode Class Destructor
//-----------------------------------------------------------------------------
BulletNode::~BulletNode(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Deletes the textures, meshes etc
//-----------------------------------------------------------------------------
void BulletNode::Shutdown(void)
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
HRESULT BulletNode::Render(void)
{
	D3DXMatrixTranslation(&_translationMatrix, _x, _y, _z);
	D3DXMatrixRotationY(&_rotationMatrixY, _angle);
	_worldMatrix = _rotationMatrixY * _translationMatrix;
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);
	_pd3dDevice->SetMaterial(&_materials);
	_mesh->DrawSubset(0);
	_boundingSphere->UpdateBoundingShape(&_worldMatrix);
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Updates the Bullet
//-----------------------------------------------------------------------------
void BulletNode::Update(void)
{
	float _bulletHeight = _terrainNode->GetHeight(_x, _z);
	
	if (_z < _maxDistance)
	{
		// 240 degrees to 180 degrees
		if(_angle < -0.80f && _angle > -1.60f || _angle > 0.80f && _angle < 1.60f)
		{
			_z -= 2.0f;
			_x += _angle;
		}

		// 180 degrees to 90 degrees
		else if(_angle < -1.60f && _angle > -2.40f || _angle > 1.60f && _angle < 2.40f)
		{
			_z -= 2.0f;
			_x += (_angle*-1);
		}

		else if (_angle > 2.4f && _angle < 4.0f)
		{
			_z += 2.0f;
			_x -= (_angle - 2.4f);
		}

		else if (_angle < -2.4f && _angle > -4.0f)
		{
			_z += 2.0f;
			_x += (_angle + 2.4f) * -1;
		}

		else
		{
			_z += 2.0f;
			_x += _angle;
		}
	}

	// Bullet has reached the end of it's travel or collided the terrain
	if(_z >= _maxDistance || _y < _bulletHeight)
	{
		_delete = true;
		_boundingSphere->SetDelete(true);
		
		//update the position vector
		_position.x = _x;
		_position.y = _y;
		_position.z = _z;

		_sGraph->AddNode(_explosion = new ExplosionNode(L"ExplosionNode", _frame, _position, _frameWorkResourcesManager),L"Parent");
	}
}

//Accessors
bool BulletNode::DeleteMe(void)
{
	return _delete;
}

void BulletNode::ExternalDeleteMe(bool deleteMe)
{
	_delete = deleteMe;
	_boundingSphere->SetDelete(true);
		
	//update the position vector
	_position.x = _x;
	_position.y = _y;
	_position.z = _z;

	_sGraph->AddNode(_explosion = new ExplosionNode(L"ExplosionNode", _frame, _position, _frameWorkResourcesManager),L"Parent");
}