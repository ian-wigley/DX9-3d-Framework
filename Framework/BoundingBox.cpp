//-----------------------------------------------------------------------------
// File: BoundingBox.cpp
// Description : This Class managers the BoundingBox
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "BoundingBox.h"

//-----------------------------------------------------------------------------
// Name: Constructor()
// Desc: BoundingBox Class Constructor
//-----------------------------------------------------------------------------
BoundingBox::BoundingBox(LPD3DXMESH mesh, LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, wstring name, float scale)
{
	_angle = 0;
	_mesh = mesh;
	_pd3dDevice = pd3dDevice;
	_x = x;
	_y = y;
	_z = z;
	_name = name;
	_type = L"Box";
	_scale = scale;
	_center = D3DXVECTOR3(0.0f,0.0f,0.0f);
	CreateBoundingBox();
}

//-----------------------------------------------------------------------------
// Name: ~BoundingBox()
// Desc: BoundingBox Class Destructor
//-----------------------------------------------------------------------------
BoundingBox::~BoundingBox(void)
{
	SAFE_RELEASE(_mesh);
}

//-----------------------------------------------------------------------------
// Name: CreateBoundingBox()
// Desc: Create an Axis Aligned Bounding Box
//-----------------------------------------------------------------------------
void BoundingBox::CreateBoundingBox(void)
{
	_mesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID **)&vertices);

	_verts.resize(_mesh->GetNumVertices());

	// Extract the Vertices to build the bounding box
	for (unsigned int i = 0; i < _verts.size(); i++)
	{
		_verts[i] = &vertices[i].x, &vertices[i].y, &vertices[i].z;
	}
	//_boxMin = lower-left corner of the bounding box
	//_boxMax = upper-right corner of the bounding box

	_mesh->UnlockVertexBuffer();

	_boxMin.x = _verts[0].x;
	_boxMax.x = _verts[0].x;
	_boxMin.y = _verts[0].y;
	_boxMax.y = _verts[0].y;
	_boxMin.z = _verts[0].z;
	_boxMax.z = _verts[0].z;

	// Calculate the minimum & maxumum x,y,z values
	for (unsigned int i = 1; i < _verts.size(); i++)
	{
		if (_verts[i].x < _boxMin.x)
		{
			_boxMin.x = _verts[i].x;
		}
		if (_verts[i].x > _boxMax.x)
		{
			_boxMax.x = _verts[i].x;
		}
		if (_verts[i].y < _boxMin.y)
		{
			_boxMin.y = _verts[i].y;
		}
		if (_verts[i].y > _boxMax.y)
		{
			_boxMax.y = _verts[i].y;
		}
		if (_verts[i].z < _boxMin.z)
		{
			_boxMin.z = _verts[i].z;
		}
		if (_verts[i].z > _boxMax.z)
		{
			_boxMax.z = _verts[i].z;
		}
	}

	_boxMax.x *= _scale;
	_boxMin.x *= _scale;
	_boxMax.y *= _scale; 
	_boxMin.y *= _scale;
	_boxMax.z *= _scale; 
	_boxMin.z *= _scale;

	_width = _boxMax.x - _boxMin.x;
	_height = _boxMax.y - _boxMin.y;
	_length = _boxMax.z - _boxMin.z;

	// Store the vertices once a bounding box has been created, creating an oriented bounding box	
	_oobCornerVertices[0] = D3DXVECTOR3(_boxMin.x, _boxMin.y, _boxMin.z);
	_oobCornerVertices[1] = D3DXVECTOR3(_boxMax.x, _boxMin.y, _boxMin.z);
	_oobCornerVertices[2] = D3DXVECTOR3(_boxMin.x, _boxMax.y, _boxMin.z);
	_oobCornerVertices[3] = D3DXVECTOR3(_boxMax.x, _boxMax.y, _boxMin.z);
	_oobCornerVertices[4] = D3DXVECTOR3(_boxMin.x, _boxMin.y, _boxMax.z);
	_oobCornerVertices[5] = D3DXVECTOR3(_boxMax.x, _boxMin.y, _boxMax.z);
	_oobCornerVertices[6] = D3DXVECTOR3(_boxMin.x, _boxMax.y, _boxMax.z);
	_oobCornerVertices[7] = D3DXVECTOR3(_boxMax.x, _boxMax.y, _boxMax.z);

	// Setup the material properties
	_material.Ambient.r = 1.0f;
	_material.Ambient.g = 1.0f;
	_material.Ambient.b = 1.0f;
	_material.Ambient.a = 0.05f;

	_material.Diffuse.r = 0.0f;
	_material.Diffuse.g = 0.0f;
	_material.Diffuse.b = 0.0f;

	// Set the Diffuse alpha value from values of 0.0f fully transparent
	// Through to 1.0f fully opaque.
	_material.Diffuse.a = 0.55f;

	_material.Specular.r = 0.0f;
	_material.Specular.g = 0.0f;
	_material.Specular.b = 0.0f;
	_material.Specular.a = 0.05f;

	_material.Emissive.r = 0.0f;
	_material.Emissive.g = 0.0f;
	_material.Emissive.b = 0.0f;
	_material.Emissive.a = 0.05f;

	_material.Power = 200.0f;

	D3DXCreateBox(_pd3dDevice, _length, _height, _width,  &_mesh, 0);
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Render the Bounding Box
//-----------------------------------------------------------------------------
HRESULT BoundingBox::Render(void)
{
	D3DXMatrixRotationY(&_rotationMatrix, _angle);
	D3DXMatrixTranslation(&_translationMatrix, _x, _y, _z);

	_worldMatrix = _translationMatrix * _rotationMatrix;

	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	_pd3dDevice->SetMaterial(&_material);

	_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_mesh->DrawSubset(0);

	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	UpdateBoundingShape(_worldMatrix);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: UpdateBoundingShape()
// Desc: Updates the Bounding Box Shape
//-----------------------------------------------------------------------------
void BoundingBox::UpdateBoundingShape(D3DXMATRIX worldMatrix)
{
	_worldMatrix = worldMatrix;
	_x = _worldMatrix._41;
	_y = _worldMatrix._42;
	_z = _worldMatrix._43;

	// Transform Oriented Bounding Box
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&_worldCornerVertices[i], &_oobCornerVertices[i], &_worldMatrix);
	}

	// Now update the AABB
	_boxMin.x = _worldCornerVertices[0].x;
	_boxMax.x = _worldCornerVertices[0].x;
	_boxMin.y = _worldCornerVertices[0].y;
	_boxMax.y = _worldCornerVertices[0].y;
	_boxMin.z = _worldCornerVertices[0].z;
	_boxMax.z = _worldCornerVertices[0].z;

	for (int i = 1; i < 8; i++)
	{
		if (_worldCornerVertices[i].x < _boxMin.x)
		{
			_boxMin.x = _worldCornerVertices[i].x;
		}
		if (_worldCornerVertices[i].x > _boxMax.x)
		{
			_boxMax.x = _worldCornerVertices[i].x;
		}
		if (_worldCornerVertices[i].y < _boxMin.y)
		{
			_boxMin.y = _worldCornerVertices[i].y;
		}
		if (_worldCornerVertices[i].y > _boxMax.y)
		{
			_boxMax.y = _worldCornerVertices[i].y;
		}
		if (_worldCornerVertices[i].z < _boxMin.z)
		{
			_boxMin.z = _worldCornerVertices[i].z;
		}
		if (_worldCornerVertices[i].z > _boxMax.z)
		{
			_boxMax.z = _worldCornerVertices[i].z;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: IsIntersecting()
// Desc: Check for intersections between boxes/boxes & boxes/spheres
//-----------------------------------------------------------------------------
bool BoundingBox::IsIntersecting(SceneNode* otherShape)
{
	 _box1 = this;
	 _box2 = (BoundingBox*)otherShape;

	//If the max Xposition of A is less than the min xposition of B they do not collide
	if ((_box1->_boxMax.x <= _box2->_boxMin.x) || (_box1->_boxMin.x >= _box2->_boxMax.x) ||
		(_box1->_boxMax.y <= _box2->_boxMin.y) || (_box1->_boxMin.y >= _box2->_boxMax.y) ||
		(_box1->_boxMax.z <= _box2->_boxMin.z) || (_box1->_boxMin.z >= _box2->_boxMax.z))
	{
		// Not intersecting
		return false;
	}
	else
	{
		// Intersecting
		return true;
	}
}

//-----------------------------------------------------------------------------
// Name: CheckDirectionX()
// Desc: Checks which side we are colliding with
//-----------------------------------------------------------------------------
bool BoundingBox::CheckDirectionX(void)
{
	if (_box1->_boxMax.x >= _box2->_boxMin.x)
	{
		return true;
	}
	else if (_box1->_boxMin.x <= _box2->_boxMax.x)
	{
		return false;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Name: CheckDirectionZ()
// Desc: Checks which side we are colliding with
//-----------------------------------------------------------------------------
bool BoundingBox::CheckDirectionZ(void)
{
	if (_box1->_boxMax.z >= _box2->_boxMin.z)
	{
		return true;
	}
	else if (_box1->_boxMin.z <= _box2->_boxMax.z)
	{
		return false;
	}
	return 0;
}


//Accessors
D3DXVECTOR3 BoundingBox::GetFrontLeftVector(void)const
{
	return D3DXVECTOR3(_boxMax.x - _width,_boxMax.y - _height,_boxMax.z);
}

D3DXVECTOR3 BoundingBox::GetFrontMidVector(void)const
{
	return D3DXVECTOR3(_boxMax.x - (_width/2),_boxMax.y - _height,_boxMax.z);
}

D3DXVECTOR3 BoundingBox::GetFrontRightVector(void)const
{
	return D3DXVECTOR3(_boxMax.x,_boxMax.y - _height,_boxMax.z);
}

D3DXVECTOR3 BoundingBox::GetRearMidVector(void)const
{
	return D3DXVECTOR3(_boxMin.x + (_width/2),_boxMax.y - _height,_boxMin.z);
}

D3DXVECTOR3 BoundingBox::GetBoxMax(void)
{
	return _boxMax;
}

D3DXVECTOR3 BoundingBox::GetBoxMin(void)
{
	return _boxMin;
}