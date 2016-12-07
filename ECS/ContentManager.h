#pragma once
//#include <cstdint>
#include <vector>
#include <fstream>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <map>
#include <memory>
#include <Windows.h>

#include "SimpleShader.h"
#include "Vertex.h"
#include "RenderingComponent.h"
#include "MeshStore.h"

class ContentManager
{
	//handle model, texture, and shader loading
	//http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		//use second solution
public:
	//default constructor (Don't Use)
	ContentManager();
	//ContentManager( ID3D11Device* device, ID3D11DeviceContext* context );
	~ContentManager();

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);

	Material LoadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName, std::string normalMapName);
	ParticleMaterial LoadParticleMaterial(std::string name, std::string samplerName, std::string vs, std::string gs, std::string ps, std::string textureName);
	MeshStore GetMeshStore(std::string);
	Material GetMaterial(std::string);
	ParticleMaterial GetParticleMaterial(std::string);
	SimpleGeometryShader* GetGShader(std::string);
	SimpleVertexShader* GetVShader(std::string);
	SimplePixelShader* GetPShader(std::string);

private:
	std::unordered_map<std::string, Material>					m_materials;	//List of materials
	std::unordered_map<std::string, ParticleMaterial>			m_particleMaterials; //list of particle materials
	std::unordered_map<std::string, MeshStore>					m_meshStores;		//List of meshes
	std::unordered_map<std::string, ID3D11SamplerState*>		m_samplers;		//List of sampler states
	std::unordered_map<std::string, ID3D11ShaderResourceView*>	m_textures;	//List of textures
	std::unordered_map<std::string, ID3D11ShaderResourceView*>	m_cubemaps;	//List of textures
	std::unordered_map<std::string, SimpleVertexShader*>		m_vshaders;		//List of vertex shaders
	std::unordered_map<std::string, SimplePixelShader*>			m_pshaders;		//List of pixel shaders
	std::unordered_map<std::string, SimpleGeometryShader*>		m_gshaders;

	ID3D11Device*								m_device;		//Pointer to the D3D11 Device
	ID3D11DeviceContext*						m_context;		//Pointer to the D3D11 Device Context

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	//Creates a mesh of the passed in .obj file and save it into a std::map  
	void CreateMeshStore(std::string objFile);
	//Creates a sampler states and save it into a std::map using the passed in name
	void CreateSamplers(std::string name, D3D11_TEXTURE_ADDRESS_MODE);
	void CreateTexture(std::wstring textureName);
	void CreateCubeMap(std::wstring cubeName);
	//Creates a vertex shader of the passed in wide string .cso file and saves it into a std::map
	void CreateVShader(std::wstring shader);
	//Creates a pixel shader of the passed in wide string .cso file and saves it into a std::map
	void CreatePShader(std::wstring shader);
	void CreateGShader(std::wstring shader);
	void FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles);
	void FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles);
};

