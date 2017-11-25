//-----------------------------------------------------------------------------
// File: FrameWorkResourceManager.cpp
// Description : This Class managers the Resources
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "FrameWorkResourceManager.h"

//-----------------------------------------------------------------------------
// Name: FrameWorkResourceManager()
// Desc: Load the Texture into memory if it doesnt already exist
//-----------------------------------------------------------------------------
FrameWorkResourceManager::FrameWorkResourceManager(Framework* _frame)
{
	_pd3dDevice = _frame->GetDirect3dDevice();
}

//-----------------------------------------------------------------------------
// Name: ~FrameWorkResourceManager()
// Desc: Load the Texture into memory if it doesnt already exist
//-----------------------------------------------------------------------------
FrameWorkResourceManager::~FrameWorkResourceManager(void)
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: Releases all memory
//-----------------------------------------------------------------------------
void FrameWorkResourceManager::Shutdown(void)
{
	if(_texture)
	{
		SAFE_RELEASE(_texture);
	}
	if(_mesh)
	{
		SAFE_RELEASE(_mesh);
	}
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}
	_meshes.clear();
	_materials.clear();
	_textures.clear();
	_heights.clear();
//	SAFE_DELETE(_meshMaterials);
}

//-----------------------------------------------------------------------------
// Name: SetTexture()
// Desc: Load the Texture into memory if it doesnt already exist
//-----------------------------------------------------------------------------
LPD3DXMESH FrameWorkResourceManager::GetMesh(wstring name , bool loadTextures)
{
	// Convert wstring to LPCWSTR
	LPCWSTR filename = name.c_str();

	// If the name not found load the mesh, texture and material
	if (_meshes.count(name) == 0 && name.length() > 0 )
	{
		// Load the mesh from the specified file
		D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM,
			_pd3dDevice, NULL,
			&_materialBuffer,NULL, &_numMaterials,
			&_mesh );

		// Insert the mesh & it's name into MAP, KEY container
		_meshes.insert(pair<wstring, LPD3DXMESH>(filename, _mesh));

		// Extract the information stored in the mesh (.x)
		d3dxMaterials = (D3DXMATERIAL*)_materialBuffer->GetBufferPointer();

		_meshMaterials = &d3dxMaterials->MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		_meshMaterials->Ambient = _meshMaterials->Diffuse;

		_materials.insert(pair<wstring, D3DMATERIAL9*>(filename, _meshMaterials));
	}

	if (!loadTextures)
	{
		size_t len = 0;
		len = (int)strlen(d3dxMaterials->pTextureFilename) + 1;
		if (len > 0)
		{
			wchar_t *unicodeString = new wchar_t[len];
			mbstowcs_s(&len, unicodeString, len, d3dxMaterials->pTextureFilename, len);

			D3DXCreateTextureFromFile(_pd3dDevice, unicodeString, &_texture);

			// Insert the texture into MAP container
			_textures.insert(pair<wstring, LPDIRECT3DTEXTURE9>(filename, _texture));
		}
	}
	else
	{
		SetTexture(name);
	}
	return _mesh;
}

//-----------------------------------------------------------------------------
// Name: SetTexture()
// Desc: Load the Texture into memory if it doesnt already exist
//-----------------------------------------------------------------------------
void FrameWorkResourceManager::SetTexture(wstring name)
{
	_textureIterator = _textures.find(name);

	if (_textureIterator == _textures.end())
	{
		// Convert wstring to LPCWSTR
		LPCWSTR filename = name.c_str();

		D3DXCreateTextureFromFile(_pd3dDevice, filename, &_texture);
		_textures.insert(pair<wstring, LPDIRECT3DTEXTURE9>(filename, _texture));
	}
}

//-----------------------------------------------------------------------------
// Name: GetTexture()
// Desc: Returns the Texture
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 FrameWorkResourceManager::GetTexture(wstring name)
{
	// Search the container for the name
	_textureIterator = _textures.find(name);

	if (_textureIterator != _textures.end())
	{
		return _texture;
	}
	else
	{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// Name: GetMaterial()
// Desc: Returns the Material
//-----------------------------------------------------------------------------
D3DMATERIAL9* FrameWorkResourceManager::GetMaterial(wstring name)
{
	// Search the container for the name
	_materialsIterator = _materials.find(name);

	if (_materialsIterator != _materials.end())
	{
		return _meshMaterials;
	}
	else
	{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// Name: GetNumberOfMatrials()
// Desc: Returns the Number Of Matrials
//-----------------------------------------------------------------------------
DWORD FrameWorkResourceManager::GetNumberOfMatrials(void)
{
	return _numMaterials;
}

//-----------------------------------------------------------------------------
// Name: GetDefaultMaterial()
// Desc: Returns the default materiel
//-----------------------------------------------------------------------------
D3DMATERIAL9 FrameWorkResourceManager::GetDefaultMaterial(void)
{
	// Setup the material properties
	_material.Ambient.r = 0.5f;
	_material.Ambient.g = 0.5f;
	_material.Ambient.b = 0.5f;
	_material.Ambient.a = 1.0f;

	_material.Diffuse.r = 1.0f;
	_material.Diffuse.g = 1.0f;
	_material.Diffuse.b = 1.0f;
	_material.Diffuse.a = 1.0f;

	_material.Specular.r = 1.0f;
	_material.Specular.g = 1.0f;
	_material.Specular.b = 1.0f;
	_material.Specular.a = 1.0f;

	_material.Emissive.r = 0.0f;
	_material.Emissive.g = 0.0f;
	_material.Emissive.b = 0.0f;
	_material.Emissive.a = 1.0f;

	_material.Power = 100.0f;

	return _material;
}

//-----------------------------------------------------------------------------
// Name: GetDefaultMaterial()
// Desc: Returns the default black materiel
//-----------------------------------------------------------------------------
D3DMATERIAL9 FrameWorkResourceManager::GetDefaultBlackMaterial(void)
{
	// Setup the material properties
	_material.Ambient.r = 0.0f;
	_material.Ambient.g = 0.0f;
	_material.Ambient.b = 0.0f;
	_material.Ambient.a = 0.0f;

	_material.Diffuse.r = 0.0f;
	_material.Diffuse.g = 0.0f;
	_material.Diffuse.b = 0.0f;
	_material.Diffuse.a = 0.0f;

	_material.Specular.r = 1.0f;
	_material.Specular.g = 1.0f;
	_material.Specular.b = 1.0f;
	_material.Specular.a = 1.0f;

	_material.Emissive.r = 0.0f;
	_material.Emissive.g = 0.0f;
	_material.Emissive.b = 0.0f;
	_material.Emissive.a = 1.0f;

	_material.Power = 100.0f;

	return 	_material;
}


//-----------------------------------------------------------------------------
// Name: LoadHeightMap()
// Desc: Load the LoadHeightMap into memory 
//-----------------------------------------------------------------------------
vector<float> FrameWorkResourceManager::LoadHeightMap(wstring filename, DWORD gridSize)
{ 
	vector<BYTE> in(gridSize * gridSize); 

	std::ifstream inFile(filename.c_str(), std::ios_base::binary); 
	if(inFile == NULL ) 
	{
		return _heights; 
	}

	inFile.read((char*)&in[0], in.size());
	inFile.close(); 

	// Normalise BYTE values to the range 0.0f - 1.0f;
	_heights.resize(in.size()); 
	for(unsigned int i = 0; i < in.size(); i++) 
	{
		_heights[i] = (float)in[i] / 255; 
	}
	return _heights; 
} 
