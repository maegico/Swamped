#pragma once
//#include <cstdint>
#include <vector>
#include <fstream>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <map>
#include <memory>

#include "Shaders.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Material.h"
#include "Mesh.h"

class ContentManager
{
	//handle model, texture, and shader loading
	//http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		//use second solution
	//also look into using shared and weak pointers with an unordered map for naming
public:
	ContentManager();
	ContentManager( ID3D11Device* device, ID3D11DeviceContext* context );
	~ContentManager();

	Material* loadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::wstring textureName);
	Mesh* getMesh(std::string);
	Material* getMaterial(std::string);

private:
	//could possibly use an unordered map
	std::map<std::string, Material*>			materials;
		//need to allow for multiple textures
	std::map<std::string, Mesh*>				meshes;
	//std::map<std::string, Shaders*>					shaders;
	std::map<std::string, ID3D11SamplerState*>	samplers;

	//move to the constructor
	//think about how to deal with multiple objects using a single vshader in this case?
	std::map<std::string, SimpleVertexShader*>	vshaders;
	std::map<std::string, SimplePixelShader*>	pshaders;
	/*std::map<std::string, SimpleComputeShader*>		cshaders;
	std::map<std::string, SimpleGeometryShader*>	gshaders;
	std::map<std::string, SimpleDomainShader*>		dshaders;
	std::map<std::string, SimpleHullShader*>		hshaders;*/

	ID3D11Device*			device;
	ID3D11DeviceContext*	context;

	void createMesh(std::string objFile);
	void createSamplers(std::string name);
	//materials don't need each shader, so look into various overloads
	//void createShaderLists(std::string name, SimpleVertexShader* vs, SimplePixelShader* ps, SimpleComputeShader* cs, SimpleGeometryShader* gs, SimpleDomainShader* ds, SimpleHullShader* hs);
	void createVShader(std::wstring shader);
	void createPShader(std::wstring shader);
	/*void createCShader(std::wstring shader);
	void createGShader(std::wstring shader);
	void createDShader(std::wstring shader);
	void createHShader(std::wstring shader);*/

	//name it load
		//have it create, save in map, and return the material
	//can do the same with meshes
	void createMaterial(std::string name, std::wstring textureImg, SimpleVertexShader* vshader, SimplePixelShader* pshader);
	//void createMaterial(std::string name, std::wstring textureImg, std::string shaderListName);
};

