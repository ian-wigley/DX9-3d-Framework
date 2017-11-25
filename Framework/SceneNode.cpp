//-----------------------------------------------------------------------------
// File: SceneNode.cpp
// Description : This Class managers each Node in the Scene
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "SceneNode.h"
#include "SceneGraph.h"

//-----------------------------------------------------------------------------
// Name: SceneNode()
// Desc: SceneNode class Constructor
//-----------------------------------------------------------------------------
SceneNode::SceneNode(void)
{
	_parentNode = NULL;
}

//-----------------------------------------------------------------------------
// Name: SceneNode()
// Desc: SceneNode class Constructor
//-----------------------------------------------------------------------------
SceneNode::SceneNode(wstring name, HWND hWnd)
{
	_name = name;
	_hWnd = hWnd;
	_delete = false;
	SetParent(this);
}

//-----------------------------------------------------------------------------
// Name: ~SceneNode()
// Desc: SceneNode class destructor
//-----------------------------------------------------------------------------
SceneNode::~SceneNode(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Initialise()
// Desc: Initialises the scene
//-----------------------------------------------------------------------------
HRESULT SceneNode::Initialise(void)
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: OnLostDevice()
// Desc: If the device has been lost
//-----------------------------------------------------------------------------
void SceneNode::OnLostDevice(void)
{
}
//-----------------------------------------------------------------------------
// Name: OnResetDevice()
// Desc: Resets the Device
//-----------------------------------------------------------------------------
void SceneNode::OnResetDevice(void)
{
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Updates the Scene
//-----------------------------------------------------------------------------
void SceneNode::Update(void)
{
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
HRESULT SceneNode::Render(void)
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Free up memory
//-----------------------------------------------------------------------------
void SceneNode::Shutdown(void)
{
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}
}


//-----------------------------------------------------------------------------
// Name: AddChild()
// Desc: Adds a Child node
//-----------------------------------------------------------------------------
bool SceneNode::AddChild(SceneNode* child)
{
	_children.insert(_children.end(), child);
	return true;
}

//-----------------------------------------------------------------------------
// Name: RemoveChild()
// Desc: Removes Child mode
//-----------------------------------------------------------------------------
bool SceneNode::RemoveChild(unsigned int child)
{
	if (child <= _children.size())
	{
		_children.erase(_children.begin() + (child));
		return true;
	}
	else
	{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// Name: RemoveAllChildren()
// Desc: Removes All Children
//-----------------------------------------------------------------------------
void SceneNode::RemoveAllChildren(void)
{
}

//-----------------------------------------------------------------------------
// Name: SetParent()
// Desc: Sets the  Parent
//-----------------------------------------------------------------------------
void SceneNode::SetParent(SceneNode* parent)
{
	_parentNode = parent;
}

//-----------------------------------------------------------------------------
// Name: GetParent()
// Desc: Gets Parent node
//-----------------------------------------------------------------------------
inline SceneNode* SceneNode::GetParent(void) const
{
	return _parentNode;
}

//-----------------------------------------------------------------------------
// Name: FindNode()
// Desc: Find a Node by name
//-----------------------------------------------------------------------------
SceneNode* SceneNode::FindNode(wstring name)
{
	return 0;
}

//-----------------------------------------------------------------------------
// Name: FindNode()
// Desc: Find a Node by node
//-----------------------------------------------------------------------------
SceneNode* SceneNode::FindNode(SceneNode* nodeToFind)
{
	nodeToFind = NULL;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: GetNodeList()
// Desc: Gets a list of Nodes
//-----------------------------------------------------------------------------
vector <SceneNode*> SceneNode::GetNodeList(void) const
{
	return _children;
}


//-----------------------------------------------------------------------------
// Name: GetName()
// Desc: Sets the node unique name
//-----------------------------------------------------------------------------
inline SceneNode* SceneNode::GetName(void) const
{
	return 0;
}

//-----------------------------------------------------------------------------
// Name: DeleteMe()
// Desc: Triggers a flag to delete object
//-----------------------------------------------------------------------------
bool SceneNode::DeleteMe(void)
{
	return _delete;
}

//-----------------------------------------------------------------------------
// Name: ExternalDeleteMe()
// Desc: Starts of the deltion process of a node
//-----------------------------------------------------------------------------
void SceneNode::ExternalDeleteMe(bool deleteMe)
{
	_delete = deleteMe;
}

//-----------------------------------------------------------------------------
// Name: ReturnName()
// Desc: Returns the Name of a node
//-----------------------------------------------------------------------------
wstring SceneNode::ReturnName(void)
{
	return _name;
}

//-----------------------------------------------------------------------------
// Name: GetType()
// Desc: Checks wheter its a sphere or box ?
//-----------------------------------------------------------------------------
wstring SceneNode::GetType(void)const
{
	return _type;
}

//-----------------------------------------------------------------------------
// Name: IsIntersecting()
// Desc: Checks wheter the objects are intersecting
//-----------------------------------------------------------------------------
bool SceneNode::IsIntersecting(SceneNode* otherShape)
{
	otherShape = NULL;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: CheckDirectionX()
// Desc: Checks which side we are colliding with
//-----------------------------------------------------------------------------
bool SceneNode::CheckDirectionX(void)
{
	return 0;
}

//-----------------------------------------------------------------------------
// Name: CheckDirectionZ()
// Desc: Checks which side we are colliding with
//-----------------------------------------------------------------------------
bool SceneNode::CheckDirectionZ(void)
{
	return 0;
}

//-----------------------------------------------------------------------------
// Name: SetPosition()
// Desc: Sets the Position
//-----------------------------------------------------------------------------
void SceneNode::SetPosition(D3DXVECTOR3 distance)
{
	_position += distance;
}