//-----------------------------------------------------------------------------
// File: BoundingSphere.cpp
// Description : This Class managers the BoundingSphere
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(void)
{
}

//-----------------------------------------------------------------------------
// Name: BoundingSphere()
// Desc: Constructor
//-----------------------------------------------------------------------------
BoundingSphere::BoundingSphere(LPD3DXMESH mesh, LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, wstring name, float scale)
{
	_x = x;
	_y = y;
	_z = z;
	_collision = false;
	_mesh = mesh;
	_pd3dDevice = pd3dDevice;
	_name = name;
	_type = L"Sphere";
	_scale = scale;
	_frame = NULL;
	_center = D3DXVECTOR3(0.0f,0.0f,0.0f);
	CreateBoundingSphere();
}

//-----------------------------------------------------------------------------
// Name: ~BoundingSphere()
// Desc: Deconstructor
//-----------------------------------------------------------------------------
BoundingSphere::~BoundingSphere(void)
{
	if (_mesh)
	{
		SAFE_RELEASE(_mesh);
	}
}

//-----------------------------------------------------------------------------
// Name: CreateBoundingSphere()
// Desc: Creates a Bounding Sphere
//-----------------------------------------------------------------------------
void BoundingSphere::CreateBoundingSphere(void)
{
	_mesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID **)&vertices);

	D3DXComputeBoundingSphere(vertices,	_mesh->GetNumVertices(), D3DXGetFVFVertexSize(_mesh->GetFVF()),	&_center, &_radius);

	_mesh->UnlockVertexBuffer();


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

	D3DXCreateSphere( _pd3dDevice, _radius * _scale , 16, 8,  &_mesh,  0);
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Renders the Bounding Sphere
//-----------------------------------------------------------------------------
HRESULT BoundingSphere::Render(void)
{
	D3DXMatrixTranslation( &_worldMatrix, _x, _y, _z);
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	_pd3dDevice->SetMaterial(&_material);

	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// Draw the bounding sphere with alpha blending.
	_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);

	_mesh->DrawSubset(0);
	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//WIREFRAME);
	_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Updates the bound sphere
//-----------------------------------------------------------------------------
void BoundingSphere::Update(void)
{
}

//-----------------------------------------------------------------------------
// Name: IsIntersecting()
// Desc: Checks for Bounding sphere to Bounding sphere collisions
//-----------------------------------------------------------------------------
//bool BoundingSphere::IsIntersecting(BoundingShape* otherShape)
bool BoundingSphere::IsIntersecting(SceneNode* otherShape)
{
	if (this->GetType() == L"Sphere" && otherShape->GetType() == L"Sphere")
	{
		BoundingSphere* sphere1 = this;
		BoundingSphere* sphere2 = (BoundingSphere*)otherShape;

		// Algorithm to test for intersecting spheres 
		// Optimised to avoid doing square roots

		D3DXVECTOR3 delta = sphere1->GetCenter() - sphere2->GetCenter();

		float distanceSquared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
		float sumRadii = sphere1->GetRadius() + sphere2->GetRadius();
		float radiiSquared = sumRadii * sumRadii;
		if (distanceSquared > radiiSquared)
		{
			// Not intersecting
			//		_collision = false;
			return false;
		}
		else if (distanceSquared < radiiSquared)
		{
			// Intersecting
			//		_collision = true;
			return true;
		}
	}
	else
	{
		BoundingSphere* _sphere = this;
		BoundingBox* _box1 = (BoundingBox*)otherShape;

		D3DXVECTOR3 boxMin = _box1->GetBoxMin();
		D3DXVECTOR3 boxMax = _box1->GetBoxMax();

		//If the max Xposition of A is less than the min xposition of B they do not collide
		if ((_sphere->_x <= boxMin.x) || (_sphere->_x >= boxMax.x) ||
			(_sphere->_y <= boxMin.y) || (_sphere->_y >= boxMax.y) ||
			(_sphere->_z <= boxMin.z) || (_sphere->_z >= boxMax.z))
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
	return 0;
}

//-----------------------------------------------------------------------------
// Name: UpdateBoundingShape()
// Desc: Update the bounding sphere's position & the mesh's position 
//-----------------------------------------------------------------------------
void BoundingSphere::UpdateBoundingShape(D3DXMATRIX* MatWorld)
{
	_x = MatWorld->_41;
	_y = MatWorld->_42;
	_z = MatWorld->_43;
	_center.x = MatWorld->_41;
	_center.y = MatWorld->_42;
	_center.z = MatWorld->_43;
}

//Accessors
D3DXVECTOR3 BoundingSphere::GetCenter(void)
{
	return _center;
}

float BoundingSphere::GetRadius(void)
{
	return _radius;
}

bool BoundingSphere::GetCollision(void)
{
	return _collision;
}

void BoundingSphere::SetDelete(bool deleteValue)
{
	_delete = deleteValue;
}

bool BoundingSphere::DeleteMe(void)
{
	return _delete;
}
