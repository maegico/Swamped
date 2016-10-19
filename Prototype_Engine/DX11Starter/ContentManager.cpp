#include "ContentManager.h"
#include <cstdarg>

ContentManager::ContentManager()
{
}

ContentManager::ContentManager(ID3D11Device* device, ID3D11DeviceContext* context)
	:device(device), context(context)
{
	device->AddRef();
	context->AddRef();

	meshes = std::map<std::string, Mesh*>();
	materials = std::map<std::string, Material*>();

	//below is placed on the stack, since I won't need them after this
	//below here is probably where I will probe the files for the needed info
	//the below isn't so terrible anymore, but still
		//these make sense though since we will go through all the files in a file location and grab all there names saving them inside the below vectors and more vectors
	std::vector<std::wstring> vshaderNames = { L"VertexShader.cso" };	//see if I can change this <- this is going to be terrible
	std::vector<std::wstring> pshaderNames = { L"PixelShader.cso" };	//see if I can change this <- this is going to be terrible
	std::vector<std::wstring> textures = { L"soilrough.png" , L"styrofoam.png" };
	std::vector<std::string> models = { "cube.obj", "cone.obj", "helix.obj" };

	createVShader(vshaderNames[0]);
	createPShader(pshaderNames[0]);

	createSamplers("sampler");

	//createShaderLists("first", vshaders["VertexShader.cso"], pshaders["PixelShader.cso"], nullptr, nullptr, nullptr, nullptr);
	//createMaterial("trial", textures[0], "first");

	//createMaterial("soilDirLight", textures[0], vshaders["VertexShader.cso"], pshaders["PixelShader.cso"]);
	//createMaterial("styroDirLight", textures[1], vshaders["VertexShader.cso"], pshaders["PixelShader.cso"]);

	//materials;

	createMesh(models[0]);
	createMesh(models[1]);
	createMesh(models[2]);
}

ContentManager::~ContentManager()
{
	for (auto i = meshes.begin(); i != meshes.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = materials.begin(); i != materials.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = samplers.begin(); i != samplers.end(); i++)
	{
		if (i->second != nullptr)
			i->second->Release();
	}
	for (auto i = vshaders.begin(); i != vshaders.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = pshaders.begin(); i != pshaders.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	
	context->Release();
	device->Release();
}

Material* ContentManager::loadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::wstring textureName)
{
	SimpleVertexShader* vshader = vshaders[vs];
	SimplePixelShader* pshader = pshaders[ps];
	ID3D11SamplerState*  sampler = samplers[samplerName];
	Material* mat;

	std::wstring outputDirPath = L"Assets/Textures/";
	outputDirPath = outputDirPath + textureName;
	const wchar_t* texturePath = outputDirPath.c_str();

	//ID3D11SamplerState*  sampler;
	ID3D11ShaderResourceView* texture;

	//D3D11_SAMPLER_DESC samplerDes;
	//memset(&samplerDes, 0, sizeof(D3D11_SAMPLER_DESC));
	////Address U, V, W
	//samplerDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	////Filter
	//samplerDes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//trilinear filtering
	//														//MaxLOD
	//samplerDes.MaxLOD = D3D11_FLOAT32_MAX;


	//HRESULT result = device->CreateSamplerState(&samplerDes, &sampler);
	//if (result != S_OK)
	//	printf("ERROR: Failed to create Sampler State.");

	HRESULT result = DirectX::CreateWICTextureFromFile(device, context, texturePath, 0, &texture);
	if (result != S_OK)
		printf("ERROR: Failed to Load Texture.");

	mat = new Material(vshader, pshader, texture, sampler);
	materials[name] = mat;
	//sampler->Release();
	texture->Release();
	return mat;
}

Mesh* ContentManager::getMesh(std::string mesh)
{
	return meshes[mesh];
}

Material * ContentManager::getMaterial(std::string name)
{
	return materials[name];
}

void ContentManager::createMesh(std::string objFile)
{
	std::string outputDirPath = "Assets/Models/";
	outputDirPath = outputDirPath + objFile;

	// File input object
	std::ifstream obj(outputDirPath.c_str());

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	std::vector<DirectX::XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			DirectX::XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the y
				v4.UV.y = 1.0f - v4.UV.y;

				// Add a whole triangle
				verts.push_back(v1);
				verts.push_back(v3);
				verts.push_back(v4);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
	}

	// Close the file and create the actual buffers
	obj.close();

	meshes[objFile] = new Mesh(&verts[0], &indices[0], verts.size(), indices.size(), device);
	//meshes.push_back(new Mesh(&verts[0], &indices[0], verts.size(), indices.size(), device));
}

void ContentManager::createSamplers(std::string name)
{
	ID3D11SamplerState*  sampler;

	D3D11_SAMPLER_DESC samplerDes;
	memset(&samplerDes, 0, sizeof(D3D11_SAMPLER_DESC));
	//Address U, V, W
	samplerDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//Filter
	samplerDes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//trilinear filtering
															//MaxLOD
	samplerDes.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result = device->CreateSamplerState(&samplerDes, &sampler);
	if (result != S_OK)
		printf("ERROR: Failed to create Sampler State.");
	else
	{
		samplers[name] = sampler;
	}
}

////might want to think on how this is being done
//void ContentManager::createShaderLists(std::string name, SimpleVertexShader* vs, SimplePixelShader* ps, SimpleComputeShader* cs, SimpleGeometryShader* gs, SimpleDomainShader* ds, SimpleHullShader* hs)
//{
//	Shaders* s = new Shaders();
//	*s = { vs, ps, cs, gs, ds, hs };
//	shaders[name] = s;
//}

void ContentManager::createVShader(std::wstring shader)
{
	std::wstring outputDirPath = L"Debug/";
	outputDirPath = outputDirPath + shader;
	//wchar_t begin[] = L"Debug/\0";
	//size_t len = wcslen(shader);
	//wchar_t* projDirFilePath = wcsncat(begin, shader, len+1);
	//Want to figure the above out!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! <- !!!!

	SimpleVertexShader* vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(outputDirPath.c_str()))
		vertexShader->LoadShaderFile(shader.c_str());

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist

	std::string shaderString(shader.begin(), shader.end());
	vshaders[shaderString] = vertexShader;

	//vshaders.push_back(vertexShader);
}

void ContentManager::createPShader(std::wstring shader)
{
	std::wstring outputDirPath = L"Debug/";
	outputDirPath = outputDirPath + shader;

	SimplePixelShader* pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(outputDirPath.c_str()))
		pixelShader->LoadShaderFile(shader.c_str());

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist

	std::string shaderString(shader.begin(), shader.end());
	pshaders[shaderString] = pixelShader;
	//pshaders.push_back(pixelShader);
}

void ContentManager::createMaterial(std::string name, std::wstring textureImg, SimpleVertexShader* vshader, SimplePixelShader* pshader)
{
	std::wstring outputDirPath = L"Assets/Textures/";
	outputDirPath = outputDirPath + textureImg;

	ID3D11SamplerState*  sampler;
	ID3D11ShaderResourceView* texture;

	D3D11_SAMPLER_DESC samplerDes;
	memset(&samplerDes, 0, sizeof(D3D11_SAMPLER_DESC));
	//Address U, V, W
	samplerDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//Filter
	samplerDes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//trilinear filtering
															//MaxLOD
	samplerDes.MaxLOD = D3D11_FLOAT32_MAX;


	HRESULT result = device->CreateSamplerState(&samplerDes, &sampler);
	if (result != S_OK)
		printf("ERROR: Failed to create Sampler State.");

	result = DirectX::CreateWICTextureFromFile(device, context, outputDirPath.c_str(), 0, &texture);
	if (result != S_OK)
		printf("ERROR: Failed to Load Texture.");

	materials[name] = new Material(vshader, pshader, texture, sampler);
	sampler->Release();
	texture->Release();
	//materials.push_back(new Material(vshader, pshader, texture, sampler));
}

//void ContentManager::createMaterial(std::string name, std::wstring textureImg, std::string shaderListName)
//{
//	std::wstring outputDirPath = L"Assets/Textures/";
//	outputDirPath = outputDirPath + textureImg;
//
//	ID3D11SamplerState*  sampler;
//	ID3D11ShaderResourceView* texture;
//
//	D3D11_SAMPLER_DESC samplerDes;
//	memset(&samplerDes, 0, sizeof(D3D11_SAMPLER_DESC));
//	//Address U, V, W
//	samplerDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	//Filter
//	samplerDes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//trilinear filtering
//															//MaxLOD
//	samplerDes.MaxLOD = D3D11_FLOAT32_MAX;
//
//
//	HRESULT result = device->CreateSamplerState(&samplerDes, &sampler);
//	if (result != S_OK)
//		printf("ERROR: Failed to create Sampler State.");
//
//	result = DirectX::CreateWICTextureFromFile(device, context, outputDirPath.c_str(), 0, &texture);
//	if (result != S_OK)
//		printf("ERROR: Failed to Load Texture.");
//
//	materials[name] = new Material(texture, sampler, shaders[shaderListName]);
//	sampler->Release();
//	texture->Release();
//}
