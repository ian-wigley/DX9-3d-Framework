#pragma once
#include "boundingsphere.h"


class BoundingBox :
	public BoundingShape
{
public:
	BoundingBox(LPD3DXMESH mesh, LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, wstring name, float scale);
	~BoundingBox(void);

	HRESULT Render(void);
	bool IsIntersecting(SceneNode* otherShape);
	void UpdateBoundingShape(D3DXMATRIX worldMatrix);

	D3DXVECTOR3 GetBoxMax(void);
	D3DXVECTOR3 GetBoxMin(void);
	D3DXVECTOR3 GetFrontLeftVector(void)const;
	D3DXVECTOR3 GetFrontMidVector(void)const;
	D3DXVECTOR3 GetFrontRightVector(void)const;
	D3DXVECTOR3 GetRearMidVector(void)const;

private:
	LPDIRECT3DDEVICE9 _pd3dDevice;
	void CreateBoundingBox(void);

	D3DXVECTOR3 _boxMin;
	D3DXVECTOR3 _boxMax;
	D3DXVECTOR3 _center;

	D3DXMATRIX _rotationMatrix;
	D3DXMATRIX _translationMatrix;

	vector<D3DXVECTOR3> _verts;

	D3DXVECTOR3	_oobCornerVertices[8];
	D3DXVECTOR3	_worldCornerVertices[8];

	float _angle;

	float _width;
	float _length;
	float _height;

	BoundingBox* _box1;
	BoundingBox* _box2;
	bool CheckDirectionX(void);
	bool CheckDirectionZ(void);
};

