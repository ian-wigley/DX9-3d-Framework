#include "CameraRender.h"
#include "Framework.h"
#include "FrameWorkResourceManager.h"
#include <iostream>
#include <fstream>

#pragma once
using namespace std;

class TerrainNode :
	public SceneNode
{
public:
	TerrainNode(void);
	~TerrainNode(void);
	TerrainNode(wstring name, Framework *_frame);
	TerrainNode(wstring name, Framework *_frame , FrameWorkResourceManager* frameResourcesManager);
	HRESULT Render(void);
	HRESULT InitialiseGrid(void);
	void GenerateVertices();
	void GenerateIndices();
	void Update(void);

	void SetCamera(CameraRender* _cameraRender);
	float GetHeight(float x, float z);

	struct COLORED_TEXTURED_VERTEX
	{
		D3DXVECTOR3 position; // The position
		D3DXVECTOR3 normal;	  // The normal
		D3DCOLOR    color;    // The colour
		FLOAT       tu, tv;   // The texture coordinates
	};

	#define D3DFVF_COLORED_TEXTURED_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

	std::vector<COLORED_TEXTURED_VERTEX> _vertices;			// List of vertices
	std::vector<DWORD>					_indices;			// List of indexes
	DWORD								_numVertices;		// The number of vertices in the grid
	DWORD								_numTriangles;		// Total number of triangles in the grid
	DWORD								_gridSize;			// Size of gird (number of rows or cells on each side)
	int									_numCellRows;		// Maximum cell number in a row (_gridSize - 1)
	int									_numCellCols;		// Maximum cell number in a column (_gridSize - 1)
	float								_spacing;			// Width of a grid square
	DWORD								_colour;			// Colour to use for wireframe drawing
	vector<float>					    _heights;			// The array of height values read from the height map

private:
	void Shutdown(void);
	void CalculateVertexNormals(void);
	HRESULT Initialise(void);

	float _height;
	float _eye;
	float _terrainStartX;
	float _terrainStartZ;
	float _v0;

	// Pointer to the custom Vertex Buffer
	IDirect3DVertexBuffer9 * g_pVB;

	// Pointer to the custom Index Buffer
	IDirect3DIndexBuffer9 *  g_pIB;

	D3DMATERIAL9		_material;

	CameraRender* _camRender;
	Framework* _frame;
	FrameWorkResourceManager* _frameWorkResourcesManager;
};

