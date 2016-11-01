#pragma once
//#include <cstdint>
#include <vector>
#include <fstream>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <map>
#include <memory>
#include <Windows.h>

//#include "Shaders.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "RenderingComponent.h"

class ContentManager
{
	//handle model, texture, and shader loading
	//http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		//use second solution
public:
	//default constructor (Don't Use)
	ContentManager();
	ContentManager( ID3D11Device* device, ID3D11DeviceContext* context );
	~ContentManager();

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);

	Material LoadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::wstring textureName);
	Mesh GetMesh(std::string);
	Material GetMaterial(std::string);


	//Unordered Map implementation was breaking, will work on it some more later\
		//did not seem to be the issue
private:
	//could possibly use an unordered map
	//also, need to allow for multiple textures
	//std::unordered_map<std::string, Material*>			m_materials;	//List of materials
	//std::unordered_map<std::string, Mesh*>				m_meshes;		//List of meshes
	//std::unordered_map<std::string, ID3D11SamplerState*>	m_samplers;		//List of sampler states
	//std::unordered_map<std::string, SimpleVertexShader*>	m_vshaders;		//List of vertex shaders
	//std::unordered_map<std::string, SimplePixelShader*>	m_pshaders;		//List of pixel shaders

	std::map<std::string, Material>			m_materials;	//List of materials
	std::map<std::string, Mesh>				m_meshes;		//List of meshes
	std::map<std::string, ID3D11SamplerState*>	m_samplers;		//List of sampler states
	std::map<std::string, SimpleVertexShader*>	m_vshaders;		//List of vertex shaders
	std::map<std::string, SimplePixelShader*>	m_pshaders;		//List of pixel shaders

	ID3D11Device*								m_device;		//Pointer to the D3D11 Device
	ID3D11DeviceContext*						m_context;		//Pointer to the D3D11 Device Context

	//Creates a mesh of the passed in .obj file and save it into a std::map  
	void CreateMesh(std::string objFile);
	//Creates a sampler states and save it into a std::map using the passed in name
	void CreateSamplers(std::string name);
	
	//Creates a vertex shader of the passed in wide string .cso file and saves it into a std::map
	void CreateVShader(std::wstring shader);
	//Creates a pixel shader of the passed in wide string .cso file and saves it into a std::map
	void CreatePShader(std::wstring shader);
	void FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles);
	void FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles);
};

