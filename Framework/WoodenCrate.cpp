//-----------------------------------------------------------------------------
// File: WoodenCrate.cpp
// Description : This Class managers the WoodenCrate
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "WoodenCrate.h"


struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	D3DXVECTOR3 normal; // The normal for the vertex
	FLOAT u, v;			// Texture coordinates
};

//-----------------------------------------------------------------------------
// Name: Constructor()
// Desc: Creates the Wooden Crate
//-----------------------------------------------------------------------------
WoodenCrate::WoodenCrate(void)
{
}

//-----------------------------------------------------------------------------
// Name: Constructor
// Desc: Creates the Wooden Crate
//-----------------------------------------------------------------------------
WoodenCrate::WoodenCrate(wstring name, Framework *_frame, float x, float z, FrameWorkResourceManager* _frameResourcesManager)
{
	_x = x;
	_y = 0;
	_z = z;
	_name = name;
	_texture = NULL;
	_scale = 1.0f;
	_pd3dDevice = _frame->GetDirect3dDevice();
	_sGraph =_frame->GetSceneGraph();
	_delete = false;
	_frameWorkResourcesManager = _frameResourcesManager;
	_frameWorkResourcesManager->SetTexture(L"woodbox.bmp");
	_terrainNode = _frame->GetTerrain();
	_y = _terrainNode->GetHeight(_x, _z) + 5.0f;
	InitialiseCrate();
}


//-----------------------------------------------------------------------------
// Name: ~WoodenCrate()
// Desc: WoodenCrate Class Destructor
//-----------------------------------------------------------------------------
WoodenCrate::~WoodenCrate(void)
{
	Shutdown();
}


//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Deletes the objects used in the Class
//-----------------------------------------------------------------------------
void WoodenCrate::Shutdown(void)
{
	SAFE_RELEASE(_texture);
	SAFE_RELEASE(_mesh);
}

//-----------------------------------------------------------------------------
// Name: InitialiseCrate()
// Desc: Initialises the Crate
//-----------------------------------------------------------------------------
HRESULT WoodenCrate::InitialiseCrate(void)
{
	_material = _frameWorkResourcesManager->GetDefaultMaterial();


	// Grab our texture from the FrameWork Resources Manager
	if(FAILED(_texture = _frameWorkResourcesManager->GetTexture(L"woodbox.bmp")))
	{
		return false;
	}

	// Create the box mesh
	D3DXCreateBox(_pd3dDevice, 10.0f, 10.0f, 10.0f, &_mesh, NULL);
	// Check that the mesh does have 12 trianges and 24 vertices (a sanity check)
	if (_mesh->GetNumFaces() != 12 || _mesh->GetNumVertices() != 24)
	{
		return false;
	}

	// Get the current vertex format for the mesh
	DWORD fvf = _mesh->GetFVF();

	if (!(fvf & D3DFVF_TEX1)) 
	{
		fvf |= D3DFVF_NORMAL;
		fvf |= D3DFVF_TEX1;
		ID3DXMesh *newMesh;
		if (FAILED(_mesh->CloneMeshFVF(0, fvf, _pd3dDevice, &newMesh))) 
		{
			return false;
	    }

	    _mesh->Release();
		_mesh = newMesh;
		// Calculate normals for all vertices
		D3DXComputeNormals(_mesh, NULL); 

		// Now go through the vertices and set the texture coordinates
		//
		// Get pointer to the vertex buffer
		LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
		if (FAILED(_mesh->GetVertexBuffer(&g_pVB)))
		{
			return false;
		}
		// and lock it
		CUSTOMVERTEX * pVertices;
		if( FAILED(g_pVB->Lock(0, 24 * sizeof(CUSTOMVERTEX), (VOID **)&pVertices, 0)))
		{
			return false;
		}
		// Now go through the 6 faces of the cube, setting the texture coordinates for each of the
		// vertices
		for (int i = 0; i < 24; i += 4)
		{
			(*pVertices).u = 0.0f;
			(*pVertices).v = 0.0f;
			pVertices++;
			(*pVertices).u = 1.0f;
			(*pVertices).v = 0.0f;
			pVertices++;
			(*pVertices).u = 1.0f;
			(*pVertices).v = 1.0f;
			pVertices++;
			(*pVertices).u = 0.0f;
			(*pVertices).v = 1.0f;
			pVertices++;
		}
		// Ensure we unlock and release the vertex buffer
		g_pVB->Unlock();
		g_pVB->Release();
	}
	_sGraph->AddNode(_boundingBox = new BoundingBox(_mesh, _pd3dDevice, _x, _y ,_z, L"boundingBox", _scale),L"Parent");
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Render() ALL Render methods should call the world transform matrix.....
// Desc: Draws the Crate
//-----------------------------------------------------------------------------
HRESULT WoodenCrate::Render(void)
{
	D3DXMatrixTranslation( &_worldMatrix, _x, _y, _z);
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	// Set texture to use
    _pd3dDevice->SetTexture( 0, _texture );
	_pd3dDevice->SetMaterial(&_material);

    _pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    _pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    _pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// Draw the box
	_mesh->DrawSubset(0);

	// Update the bounding Box
	_boundingBox->UpdateBoundingShape(_worldMatrix);

	return S_OK;
}