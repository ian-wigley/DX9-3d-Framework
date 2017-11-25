#pragma once
#include "boundingshape.h"
#include "BoundingBox.h"
#include "Framework.h"

class BoundingSphere :
	public BoundingShape
{
public:
	BoundingSphere(void);
	BoundingSphere(LPD3DXMESH mesh, LPDIRECT3DDEVICE9 pd3dDevice, float x, float y, float z, wstring name, float scale);
	~BoundingSphere(void);
	
	HRESULT Render(void);
	void Update(void);

	// Intersection testing
//	bool IsIntersecting(BoundingShape* otherShape);

	bool IsIntersecting(SceneNode* otherShape);



	void UpdateBoundingShape(D3DXMATRIX* matWorld);
	bool GetCollision(void);

	void SetDelete(bool deleteValue);
	bool DeleteMe(void);
	float GetRadius(void);
	D3DXVECTOR3 GetCenter(void);

private:
	void CreateBoundingSphere(void);
	D3DXVECTOR3 _center;
	bool _collision;
	float _radius;

	Framework* _frame;
};

