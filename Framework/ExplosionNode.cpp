//-----------------------------------------------------------------------------
// File: ExplosionNode.cpp
// Description : This Class managers a billboard showing animated explosion
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "ExplosionNode.h"

//-----------------------------------------------------------------------------
// Name: Constructor
// Desc: Creates the ExplosionNode
//-----------------------------------------------------------------------------
ExplosionNode::ExplosionNode(void)
{
}

//-----------------------------------------------------------------------------
// Name: Constructor
// Desc: Creates the ExplosionNode
//-----------------------------------------------------------------------------
ExplosionNode::ExplosionNode(wstring name, Framework* frame, D3DXVECTOR3 position, FrameWorkResourceManager* frameResourcesManager)
{
	_delete = false;
	_position = position;
	_angle = 0;
	_x = _position.x - 5.0f;
	_y = _position.y;
	_z = _position.z;
	_u1 = 0.0f;
	_u2 = 0.07f;
	_name = name;
	_frame = frame;
	_texture = NULL;
	_collsion = false;
	_pd3dDevice = _frame->GetDirect3dDevice();
	_frameWorkResourcesManager = frameResourcesManager;
	_frameWorkResourcesManager->SetTexture(L"Explosion.png");
	InitialiseExplosion();
}

//-----------------------------------------------------------------------------
// Name: ~ExplosionNode
// Desc: ExplosionNode Class Destructor
//-----------------------------------------------------------------------------
ExplosionNode::~ExplosionNode(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Deletes the objects used in the Class
//-----------------------------------------------------------------------------
void ExplosionNode::Shutdown(void)
{
	if(_texture)
	{
		SAFE_RELEASE(_texture);
	}
	if(_mesh)
	{
		SAFE_DELETE(_mesh);
	}
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}

}



//-----------------------------------------------------------------------------
// Name: InitialiseExplosion()
// Desc: Initialises the BillBoard
//-----------------------------------------------------------------------------
HRESULT ExplosionNode::InitialiseExplosion(void)
{
	_material = _frameWorkResourcesManager->GetDefaultMaterial();

	CUSTOMVERTEX g_Vertices[] =
	{
		{ 0.0f, -5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u1, 1.0f, },
		{ 0.0f,  5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u1, 0.0f, },
		{10.0f, -5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u2, 1.0f, },
		{10.0f,  5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u2, 0.0f, },
	};

	// Create the vertex buffer.
	if( FAILED( _pd3dDevice->CreateVertexBuffer(4 * sizeof( CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices;
	if( FAILED( g_pVB->Lock(0, sizeof( g_Vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}
	memcpy(pVertices, g_Vertices, sizeof(g_Vertices));

	g_pVB->Unlock();


	// Grab our texture from the FrameWork Resources Manager
	if(FAILED(_texture = _frameWorkResourcesManager->GetTexture(L"Explosion.png")))
	{
		return false;
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the BillBoard
//-----------------------------------------------------------------------------
HRESULT ExplosionNode::Render(void)
{

	D3DXMatrixTranslation( &_worldMatrix, _x, _y, _z);
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// Set texture to use
	_pd3dDevice->SetTexture( 0, _texture );
	_pd3dDevice->SetMaterial(&_material);

	_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//Enables Alpha Blending
	_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

	// Draw the billboard
	_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Updates the BillBoard
//-----------------------------------------------------------------------------
void ExplosionNode::Update(void)
{
	if (_u2 < 1)
	{
		_u1 += 0.062f;
		_u2 += 0.062f;
	}
	else
	{
		_delete = true;
	}

	CUSTOMVERTEX g_Vertices[] =
	{
		{ 0.0f, -5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u1, 1.0f, },
		{ 0.0f,  5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u1, 0.0f, },
		{10.0f, -5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u2, 1.0f, },
		{10.0f,  5.0f, 0.0f, D3DXVECTOR3(1.0f,1.0f,1.0f), _u2, 0.0f, },
	};

	// Create the vertex buffer.
	_pd3dDevice->CreateVertexBuffer(4 * sizeof( CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL);

	// Fill the vertex buffer.
	VOID* pVertices;
	g_pVB->Lock(0, sizeof( g_Vertices), (void**)&pVertices, 0);
	memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
	g_pVB->Unlock();
}

//Accessors
bool ExplosionNode::DeleteMe(void)
{
	return _delete;
}