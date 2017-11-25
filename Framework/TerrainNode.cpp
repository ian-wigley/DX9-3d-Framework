//-----------------------------------------------------------------------------
// File: TerrainNode.cpp
// Description : This Class managers the Terrain
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "TerrainNode.h"

//-----------------------------------------------------------------------------
// Name: TerrainNode()
// Desc: TerrainNode Class Constructor
//-----------------------------------------------------------------------------
TerrainNode::TerrainNode(void)
{
	_gridSize = 256;
	_numCellCols = 256;
	_numCellRows = 256;
	_spacing = 10;
	g_pVB = 0;
	InitialiseGrid();
}

//-----------------------------------------------------------------------------
// Name: SetTexture()
// Desc: TerrainNode Class Constructor
//-----------------------------------------------------------------------------
TerrainNode::TerrainNode(wstring name, Framework *_frame , FrameWorkResourceManager* frameResourcesManager)
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;

	_v0 = 0.0f;
	_delete = false;
	_texture = NULL; 
	_height = 100.0f;

	_gridSize = 256;
	_numCellCols = 255;
	_numCellRows = 255;
	_numVertices = _gridSize * _gridSize;

	_spacing = 10;
	_numTriangles = _numCellCols * _numCellRows * 2;
	_name = name;

	_terrainStartX = 0;
	_terrainStartZ = 0;

	_hWnd = _frame->GetHandle();
	_pd3dDevice = _frame->GetDirect3dDevice();
	_frameWorkResourcesManager = frameResourcesManager;
	_frameWorkResourcesManager->SetTexture(L"volcano.bmp");
	_texture = 	_frameWorkResourcesManager->GetTexture(L"volcano.bmp");
	_heights = _frameWorkResourcesManager->LoadHeightMap(L"volcano.raw",_gridSize);
	InitialiseGrid();
	Initialise();
}

//-----------------------------------------------------------------------------
// Name: ~TerrainNode()
// Desc: TerrainNode Class Destructor
//-----------------------------------------------------------------------------
TerrainNode::~TerrainNode(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Deletes the objects used in the Terrain
//-----------------------------------------------------------------------------
void TerrainNode::Shutdown(void)
{
	if (_heights.size() != 0)
	{
		_heights.clear();
	}

	if (_vertices.size() != 0)
	{
		_vertices.clear();
	}

	if (_indices.size() != 0)
	{
		_indices.clear();
	}

	SAFE_RELEASE(_texture);
	SAFE_RELEASE(g_pVB);
	SAFE_RELEASE(g_pIB);

	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}
}

//-----------------------------------------------------------------------------
// Name: InitialiseGrid()
// Desc: Initialises the Grid
//-----------------------------------------------------------------------------
HRESULT TerrainNode::InitialiseGrid(void)
{
	// Initialise the DirectX Device
	this->Initialise();

	GenerateVertices();
	GenerateIndices();
	CalculateVertexNormals();

	// Create the vertex buffer.
	if( FAILED( _pd3dDevice->CreateVertexBuffer( _numVertices * sizeof( COLORED_TEXTURED_VERTEX ),
		0, D3DFVF_COLORED_TEXTURED_VERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
	{
		return E_FAIL;
	}
	//Lock the Vertex buffer and populate it with our vertices data
	COLORED_TEXTURED_VERTEX* _vertex = NULL;
	if( FAILED(g_pVB->Lock(0, 0, (void**)& _vertex,0 )))
	{
		return E_FAIL;
	}
	for (DWORD i = 0; i < _vertices.size(); i++)
	{
		_vertex[i] = _vertices[i];
	}
	g_pVB->Unlock();

	// Create the index buffer
	if (FAILED(_pd3dDevice->CreateIndexBuffer(_numTriangles * 3 *sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIB,
		NULL)))
	{
		return E_FAIL;
	}


	WORD* indices = NULL;
	// lock indexBuffer and load the indices into it
	if (FAILED(g_pIB->Lock(0, 0, (void**)&indices, 0)))
	{
		return E_FAIL;
	}
	for (DWORD i = 0; i < _indices.size(); i++)
	{
		indices[i] = (WORD)_indices[i];
	}
	g_pIB->Unlock(); 

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GenerateVertices()
// Desc: Generate the Vertices
//-----------------------------------------------------------------------------
void TerrainNode::GenerateVertices()
{
	_vertices.resize(_numVertices);

	float width = (float)_numCellCols * _spacing;
	float depth = (float)_numCellRows * _spacing;

	float xOffset = -width * 0.5f;
	float zOffset = depth * 0.5f;


	_terrainStartX = xOffset;
	_terrainStartZ = zOffset;


	float uIncrement = 1.0f / _numCellRows;
	float vIncrement = 1.0f / _numCellCols;

	// Set up the vertex array.  For now, we set the y co-ordinate to 0 (we will adjust this when we
	// apply the height map
	int vertex = 0;
	for( float i = 0; i < _gridSize; ++i)
	{
		for (float j = 0; j < _gridSize; ++j)
		{
			_vertices[vertex].position.x = j *  _spacing + xOffset;
			_vertices[vertex].position.z = -i * _spacing + zOffset;
			_vertices[vertex].position.y = _heights[vertex] * _height; 
			_vertices[vertex].normal	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			_vertices[vertex].color		 = _colour;
			_vertices[vertex].tu		 = j * uIncrement;
			_vertices[vertex].tv		 = i * vIncrement;
			++vertex;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: GenerateIndices()
// Desc: Generates the Indices
//-----------------------------------------------------------------------------
void TerrainNode::GenerateIndices()
{
	_indices.resize(_numTriangles * 3);

	int index = 0;
	for (DWORD i = 0; i< (DWORD)_numCellRows; ++i )
	{
		for ( DWORD j = 0; j < (DWORD)_numCellCols; ++j )
		{
			// First triangle
			_indices[index]     = (i * _gridSize) + j;
			_indices[index + 1] = (i * _gridSize) + (j + 1);
			_indices[index + 2] = ((i+1) * _gridSize) + j;

			// Second triangle
			_indices[index + 3] = ((i+1) * _gridSize) + j;
			_indices[index + 4] = (i * _gridSize) + (j + 1);
			_indices[index + 5] = ((i+1) * _gridSize) + (j + 1);

			index += 6;
		}
	}
}


//-----------------------------------------------------------------------------
// Name: CalculateVertexNormals()
// Desc: Calculates the Vertex Normals
//-----------------------------------------------------------------------------
void TerrainNode::CalculateVertexNormals(void)
{
	// Compute normals with finite difference over interior nodes.
	float invTwoDX = 1.0f / (2.0f *_spacing);
	float invTwoDZ = 1.0f / (2.0f *_spacing);
	for(DWORD i = 1; i < (unsigned)_numCellRows-1; ++i)
	{
		for(DWORD j = 1; j < (unsigned)_numCellCols-1; ++j) 
		{    
			float t = _heights[(i-1+j)];
			float b = _heights[(i+1+j)];
			float l = _heights[(i+j-1)];
			float r = _heights[(i+j+1)];
			D3DXVECTOR3 _tanZ(0.0f, (t-b)*invTwoDZ, 1.0f); 
			D3DXVECTOR3 _tanX(1.0f, (r-l)*invTwoDX, 0.0f); 
			D3DXVECTOR3 _n; 
			D3DXVec3Cross(&_n, &_tanZ, &_tanX); 
			D3DXVec3Normalize(&_n, &_n); 
			_vertices[i * _numCellCols+j].normal = _n; 
		} 
	}
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
HRESULT TerrainNode::Render(void)
{

	_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF0000FF, 1.0f, 0);
	_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	D3DXMatrixTranslation( &_worldMatrix, _x, _y, _z);
	_pd3dDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	// Set texture to render
	_pd3dDevice->SetTexture( 0, _texture );
	_pd3dDevice->SetMaterial(&_material);

	// Render the vertex buffer contents
	_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( COLORED_TEXTURED_VERTEX ) );

	// Specify the index buffer
	_pd3dDevice->SetIndices(g_pIB);
	_pd3dDevice->SetFVF( D3DFVF_COLORED_TEXTURED_VERTEX);
	if( FAILED(_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _numVertices, 0, _numTriangles)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Updates the Terrain
//-----------------------------------------------------------------------------
void TerrainNode::Update(void)
{
	_camRender->Update();
	_camRender->Render();
}

//-----------------------------------------------------------------------------
// Name: Initialise()
// Desc: Initialises the Terrain
//-----------------------------------------------------------------------------
HRESULT TerrainNode::Initialise(void)
{
	_material = _frameWorkResourcesManager->GetDefaultMaterial();
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SetCamera()
// Desc: Sets a pointer to the Camera
//-----------------------------------------------------------------------------
void TerrainNode::SetCamera(CameraRender* _cameraRender)
{
	_camRender = _cameraRender;
}

//-----------------------------------------------------------------------------
// Name: GetHeight()
// Desc: Returns the Height of the Terrain at a particular location
//-----------------------------------------------------------------------------
float TerrainNode::GetHeight(float x, float z)
{
	//Calculate which cell we are in
	int indexBase = 0;
	int _cellX = (int)((x - _terrainStartX) / _spacing);
	int _cellZ = (int)((_terrainStartZ - z) / _spacing);

	int verticesIndex = _cellZ * _gridSize + _cellX;
	float _dx = x - _vertices[verticesIndex].position.x;
	float _dz = z - _vertices[verticesIndex].position.z;
	// Get the first index of the triangle we are positioned in

	// We are in triangle V0,V1,V2
	if (_dz > _dx)
	{
		indexBase = (_cellZ * _numCellCols + _cellX) * 6;
	}
	// We are in triangle V0,V2,V3
	else
	{
		indexBase = (_cellZ * _numCellCols + _cellX) * 6 + 3;
	}

	// v0---v1
	// |\   | <-dx->
	// | \  |   /\ 
	// |  \ |   dz
	// |   \|   \/
	// v3---v2

	int index0 = _indices[indexBase];
	int index1 = _indices[indexBase + 1];
	int index2 = _indices[indexBase + 2];

	D3DXVECTOR3 _point(_dx, 0.0f, _dz);

	// Calculate the Triangle Normal
	D3DXVECTOR3 _u(_vertices[index1].position.x - _vertices[index0].position.x, 
		_vertices[index1].position.y - _vertices[index0].position.y,
		_vertices[index1].position.z - _vertices[index0].position.z);

	D3DXVECTOR3 _v(_vertices[index2].position.x - _vertices[index0].position.x, 
		_vertices[index2].position.y - _vertices[index0].position.y,
		_vertices[index2].position.z - _vertices[index0].position.z);

	D3DXVECTOR3 _normal;
	D3DXVec3Cross(&_normal, &_u, &_v);
	D3DXVec3Normalize(&_normal, &_normal);

	float Nx = _normal.x;
	float Nz = _normal.z;
	float Ny = _normal.y;

	_v0 = _vertices[index0].position.y;

	return _point.y = _v0 + ((Nx * _dx) + (Nz * _dz)/ -Ny);
}
