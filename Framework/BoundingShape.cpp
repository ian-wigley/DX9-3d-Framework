//-----------------------------------------------------------------------------
// File: BoundingShape.cpp
// Description : This Class managers the BoundingShape
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "BoundingShape.h"

//-----------------------------------------------------------------------------
// Name: Constructor()
// Desc: BoundingShape Class Constructor
//-----------------------------------------------------------------------------
BoundingShape::BoundingShape(void)
{
	_delete = false;
}

//-----------------------------------------------------------------------------
// Name: ~BoundingShape()
// Desc: BoundingShape Class Destructor
//-----------------------------------------------------------------------------
BoundingShape::~BoundingShape(void)
{
}

//-----------------------------------------------------------------------------
// Name: IsIntersecting()
// Desc: Check for intersections between boxes/boxes & boxes/spheres
//-----------------------------------------------------------------------------
bool BoundingShape::IsIntersecting(SceneNode* otherShape)
{
	otherShape = NULL;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: UpdateBoundingShape()
// Desc: Updates the Bounding Box Shape
//-----------------------------------------------------------------------------
void BoundingShape::UpdateBoundingShape(D3DXMATRIX* worldMatrix)
{
	worldMatrix = NULL;
}

//-----------------------------------------------------------------------------
// Name: GetType()
// Desc: Checks wheter its a sphere or box ?
//-----------------------------------------------------------------------------
wstring BoundingShape::GetType(void)const
{
	return _type;
}

//Accessors
D3DXVECTOR3* BoundingShape::GetCenter(void)
{
	return _center;
}

float BoundingShape::GetRadius(void)
{
	return _radius;
}

void BoundingShape::SetDelete(bool deleteValue)
{
	_delete = deleteValue;
}

bool BoundingShape::DeleteMe(void)
{
	return _delete;
}

wstring BoundingShape::GetName(void)
{
	return _name;
}