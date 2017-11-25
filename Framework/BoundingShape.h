#pragma once
#include "scenenode.h"
class BoundingSphere;

class BoundingShape :
	public SceneNode
{
public:
	BoundingShape(void);
	~BoundingShape(void);
	void AddBoundingSpheres(BoundingSphere* sphere);
	
	// Intersection testing
//	virtual bool IsIntersecting(BoundingShape* otherShape);
	virtual bool IsIntersecting(SceneNode* otherShape);

	wstring GetType(void)const;
	float GetRadius(void);
	D3DXVECTOR3* GetCenter(void);

	virtual void SetDelete(bool deleteValue);
	virtual bool DeleteMe(void);

	wstring GetName(void);

protected:
	LPD3DXMESH _mesh;
	LPDIRECT3DDEVICE9 _pd3dDevice;

	D3DMATERIAL9 _material;
	D3DXVECTOR3* vertices;
	D3DXVECTOR3* _center;
	D3DXMATRIX _worldMatrix;

	float _radius;
	float _x;
	float _y;
	float _z;

	bool _collision;

//	wstring _name;
	//wstring _type;

	virtual void UpdateBoundingShape(D3DXMATRIX* matWorld);
};

