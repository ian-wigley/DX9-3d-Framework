#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include <d3d9types.h>
#include <d3dx9math.h>

#pragma once
using namespace std;

class BoundingShape;

class SceneNode
{
public:
	SceneNode(void);
	SceneNode(wstring name);
	SceneNode(wstring name, HWND hWnd);
	~SceneNode(void);

	// Core methods 
	virtual HRESULT Initialise(void);
	virtual void OnLostDevice(void); 
	virtual void OnResetDevice(void); 
	virtual void Update(void);          
	virtual HRESULT Render(void);          
	virtual void Shutdown(void); 

	// Intersection testing
	virtual bool IsIntersecting(SceneNode* otherShape);
	wstring GetType(void)const;

	// Events 
	virtual void OnAdded(void) {} ;
	virtual void OnRemoved(void){} ;

	// Child changes 
	bool AddChild(SceneNode* child); 
	bool RemoveChild(unsigned int child);
	void RemoveAllChildren(void); 

	void SetParent(SceneNode* parent);         
	inline SceneNode* GetParent(void) const;  
	inline SceneNode* GetName(void) const;

	//Graph searching 
	SceneNode* FindNode(wstring name);
	SceneNode* FindNode(SceneNode* nodeToFind); 

	vector<SceneNode*> GetNodeList(void) const;

	virtual void ExternalDeleteMe(bool deleteMe);

	virtual bool CheckDirectionX(void);
	virtual bool CheckDirectionZ(void);

	bool DeleteMe(void);

	wstring ReturnName(void);

	virtual void SetPosition(D3DXVECTOR3 distance);

protected:
	HWND _hWnd;
	wstring _name;               

	#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }

	 // converts from degrees to radians
	#define DEGTORAD(degree) ((D3DX_PI / 180.0f) * (degree))
	// converts from radians to degrees
	#define RADTODEG(radian) ((180.0f / D3DX_PI) * (radian)) 

	LPDIRECT3DDEVICE9   _pd3dDevice;
	LPD3DXBUFFER materialBuffer;
	LPD3DXMESH _mesh;
	LPDIRECT3DTEXTURE9 _texture;
	D3DMATERIAL9*	   _material;

	D3DXMATRIX _rotationMatrixX;
	D3DXMATRIX _rotationMatrixY;
	D3DXMATRIX _rotationMatrixZ;
	
	D3DXMATRIX _scalingingMatrix;
	D3DXMATRIX _translationMatrix;
	D3DXMATRIX _worldMatrix;
	
	D3DXMATRIX _viewMatrix;

	D3DXVECTOR3 _up;
	D3DXVECTOR3 _lookAt;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _position;

	D3DXMATRIX yawMatrix;
	D3DXMATRIX pitchMatrix;
	D3DXMATRIX rollMatrix;

	float _cameraSpeed;
    float _angle;

	float _yaw;						// Degrees of yaw (in radians)
	float _pitch;					// Degrees of pitch (in radians)
	float _roll;					// Degrees of roll (in radians)

	float _x;
	float _y;
	float _z;

	float _scale;

	bool _collsion;
	bool _delete;

	wstring _type;

	DWORD _numberOfMaterials;

private:
	SceneNode* _parentNode;			// The parent of the scene node 
	vector<SceneNode*>  _children;  // The scene nodes child nodes
};