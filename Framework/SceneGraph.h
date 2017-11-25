#include "BoundingShape.h"
#include "SceneNode.h"
#include <d3dx9.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <string>

#pragma once
using namespace std;

class SceneGraph : public SceneNode
{
public:
	SceneGraph(void);
	~SceneGraph(void);
	SceneGraph(	LPDIRECT3DDEVICE9 pd3dDevice);

    HRESULT Initialise(void); 
    void OnLostDevice(void); 
    void OnResetDevice(void); 
    void Update(void); 
    HRESULT Render(void); 
    void Shutdown(void); 
 
    bool AddNode(SceneNode* node, wstring parentNode); 
	bool RemoveNode(unsigned int node);

    SceneNode* FindNode(wstring name); 
    SceneNode* FindNode(SceneNode* nodeToFind); 

protected:
	#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }

private:
	int FindANode(wstring name);
	
    // The scene root node
    SceneNode* _rootNode;
	vector <SceneNode*> _nodes;
};