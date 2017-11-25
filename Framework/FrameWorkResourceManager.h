#include "Framework.h"
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <d3dx9.h>
#include <iostream>
#include <fstream>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#pragma once

using namespace std;


class FrameWorkResourceManager
{
public:
	FrameWorkResourceManager(void);
	FrameWorkResourceManager(Framework* _frame);
	~FrameWorkResourceManager(void);
	void Shutdown(void);
	LPD3DXMESH GetMesh(wstring name , bool loadTextures= true);
	LPDIRECT3DTEXTURE9 GetTexture(wstring name);

	void SetTexture(wstring name);

	HRESULT LoadTextures(wstring filename);
	vector<float> LoadHeightMap(wstring filename, DWORD gridSize);

	D3DMATERIAL9* GetMaterial(wstring name);

	DWORD GetNumberOfMatrials(void);
	D3DMATERIAL9 GetDefaultMaterial(void);
	D3DMATERIAL9 GetDefaultBlackMaterial(void);
private:

	LPCWSTR StringFormat(wstring name);

	map<wstring, LPD3DXMESH> _meshes;
	map<wstring, D3DMATERIAL9*> _materials;
	map<wstring, LPDIRECT3DTEXTURE9> _textures;

	// Specify an iterator
	map<wstring, LPD3DXMESH>::iterator _meshIterator;
	map<wstring, D3DMATERIAL9*>::iterator _materialsIterator;
	map<wstring, LPDIRECT3DTEXTURE9>::iterator _textureIterator;

	LPD3DXMESH _mesh;
	D3DMATERIAL9* _meshMaterials;
	LPDIRECT3DTEXTURE9 _texture;

	LPDIRECT3DDEVICE9 _pd3dDevice;
	LPD3DXBUFFER _materialBuffer;
	DWORD _numMaterials;

	D3DXMATERIAL* d3dxMaterials;
	D3DMATERIAL9 _material;


	void SetMultipleMaterials(void);
	vector<D3DMATERIAL9*>  _multipleMaterials;

	// The array of height values read from the height map
	vector<float> _heights;			

	//Todo
	// UnzipResources();
	//
};

