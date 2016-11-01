#include "ContentManager.h"
#include <cstdarg>

using namespace std;

ContentManager::ContentManager()
{
}

ContentManager::ContentManager(ID3D11Device* device, ID3D11DeviceContext* context)
	:m_device(device), m_context(context)
{
	device->AddRef();
	context->AddRef();

	//m_meshes = std::unordered_map<std::string, Mesh*>();
	//m_materials = std::unordered_map<std::string, Material*>();

	////below is placed on the stack, since I won't need them after this
	////below here is probably where I will probe the files for the needed info
	////the below isn't so terrible anymore, but still
	//	//these make sense though since we will go through all the files in a file location and grab all there names saving them inside the below vectors and more vectors
	//std::vector<std::wstring> vshaderNames = { L"VertexShader.cso" };	//see if I can change this <- this is going to be terrible
	//std::vector<std::wstring> pshaderNames = { L"PixelShader.cso" };	//see if I can change this <- this is going to be terrible
	//std::vector<std::wstring> textures = { L"soilrough.png" , L"styrofoam.png" };
	//std::vector<std::string> models = { "cube.obj", "cone.obj", "helix.obj" };

	//CreateVShader(vshaderNames[0]);
	//CreatePShader(pshaderNames[0]);

	//CreateSamplers("sampler");

	//CreateMesh(models[0]);
	//CreateMesh(models[1]);
	//CreateMesh(models[2]);
}

ContentManager::~ContentManager()
{
	for (auto i = m_samplers.begin(); i != m_samplers.end(); i++)
	{
		if (i->second != nullptr)
			i->second->Release();
	}
	for (auto i = m_vshaders.begin(); i != m_vshaders.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = m_pshaders.begin(); i != m_pshaders.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	
	m_context->Release();
	m_device->Release();
}

void ContentManager::Init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	device->AddRef();
	context->AddRef();

	m_device = device;
	m_context = context;

	/*m_materials = std::unordered_map<std::string, Material*>();
	m_meshes = std::unordered_map<std::string, Mesh*>();
	m_samplers = std::unordered_map<std::string, ID3D11SamplerState*>();
	m_vshaders = std::unordered_map<std::string, SimpleVertexShader*>();
	m_pshaders = std::unordered_map<std::string, SimplePixelShader*>();*/
	m_materials = std::map<std::string, Material>();
	m_meshes = std::map<std::string, Mesh>();
	m_samplers = std::map<std::string, ID3D11SamplerState*>();
	m_vshaders = std::map<std::string, SimpleVertexShader*>();
	m_pshaders = std::map<std::string, SimplePixelShader*>();
	

	//below is placed on the stack, since I won't need them after this
	//below here is probably where I will probe the files for the needed info
	//the below isn't so terrible anymore, but still
	//these make sense though since we will go through all the files in a file location and grab all there names saving them inside the below vectors and more vectors
	std::vector<std::wstring> vshaderNames;	//see if I can change this <- this is going to be terrible
	std::vector<std::wstring> pshaderNames;	//see if I can change this <- this is going to be terrible
	//std::vector<std::wstring> textures;
	std::vector<std::string> models;

	FindFilesInFolderWSTR(L"VertexShaders", vshaderNames);
	FindFilesInFolderWSTR(L"PixelShaders", pshaderNames);
	FindFilesInFolder(L"assets/Models", models);

	CreateSamplers("sampler");

	//The below isn't creating the shaders correctly
	for (unsigned int i = 0; i < vshaderNames.size(); i++)
	{
		CreateVShader(vshaderNames[i]);
	}
	for (unsigned int i = 0; i < pshaderNames.size(); i++)
	{
		CreatePShader(pshaderNames[i]);
	}

	for (unsigned int i = 0; i < models.size(); i++)
	{
		CreateMesh(models[i]);
	}

	LoadMaterial("TestMaterial", "sampler", "VertexShader.cso", "PixelShader.cso", L"soilrough.png");
}

//Should I hold a bunch of textures in CM or create on construction of a material
Material ContentManager::LoadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::wstring textureName)
{
	SimpleVertexShader* vshader = m_vshaders[vs];
	SimplePixelShader* pshader = m_pshaders[ps];
	ID3D11SamplerState*  sampler = m_samplers[samplerName];
	Material mat;

	std::wstring outputDirPath = L"Assets/Textures/";
	outputDirPath = outputDirPath + textureName;
	const wchar_t* texturePath = outputDirPath.c_str();

	//do I put these by themselves and hold them in my CM?????
	ID3D11ShaderResourceView* texture;

	HRESULT result = DirectX::CreateWICTextureFromFile(m_device, m_context, texturePath, 0, &texture);
	if (result != S_OK)
		printf("ERROR: Failed to Load Texture.");

	mat = { vshader, pshader, texture, sampler };//new Material(vshader, pshader, texture, sampler);
	m_materials[name] = mat;
	return mat;
}

Mesh ContentManager::GetMesh(std::string mesh)
{
	return m_meshes[mesh];
}

Material ContentManager::GetMaterial(std::string name)
{
	return m_materials[name];
}

void ContentManager::CreateMesh(std::string objFile)
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

	//m_meshes[objFile] = new Mesh(&verts[0], &indices[0], verts.size(), indices.size(), m_device);
	size_t vertCount = verts.size();
	size_t indCount = indices.size();
	ID3D11Buffer * vertexBuffer = 0;
	ID3D11Buffer * indexBuffer = 0;
	
	// Create the VERTEX BUFFER description
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertCount;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = &verts[0];

	// Actually create the buffer with the initial data
	m_device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	// Create the INDEX BUFFER description
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * indCount;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = &indices[0];

	// Actually create the buffer with the initial data
	m_device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);

	m_meshes[objFile] = { vertexBuffer, indexBuffer, indCount };
}

void ContentManager::CreateSamplers(std::string name)
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

	HRESULT result = m_device->CreateSamplerState(&samplerDes, &sampler);
	if (result != S_OK)
		printf("ERROR: Failed to create Sampler State.");
	else
		m_samplers[name] = sampler;
}

void ContentManager::CreateVShader(std::wstring shader)
{
	std::wstring outputDirPath = L"VertexShaders/";
	outputDirPath = outputDirPath + shader;
	//wchar_t begin[] = L"Debug/\0";
	//size_t len = wcslen(shader);
	//wchar_t* projDirFilePath = wcsncat(begin, shader, len+1);
	//Want to figure the above out!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! <- !!!!

	SimpleVertexShader* vertexShader = new SimpleVertexShader(m_device, m_context);
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
	m_vshaders[shaderString] = vertexShader;
}

void ContentManager::CreatePShader(std::wstring shader)
{
	std::wstring outputDirPath = L"PixelShaders/";
	outputDirPath = outputDirPath + shader;

	SimplePixelShader* pixelShader = new SimplePixelShader(m_device, m_context);
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
	m_pshaders[shaderString] = pixelShader;
}

//I could make the below better, by

//I took the basic code from below and modified it to work for both UNICODE and non-UNICODE character sets
//Got it from: http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void ContentManager::FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles)
{
#ifdef UNICODE
	std::wstring path = folder + L"/*.*";
	
	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring temp1(fd.cFileName);
				std::string temp2(temp1.begin(), temp1.end());
				listOfFiles.push_back(temp2);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

#else
	
	std::wstring path = folder + L"/*.*";
	std::string pathSTR(path.begin(), path.end());
	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(pathSTR.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				std::string temp(fd.cFileName);
				listOfFiles.push_back(temp);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
#endif
}

//I didn't code this
//Got it from: http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void ContentManager::FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles)
{
#ifdef UNICODE
	std::wstring path = folder + L"/*.*";

	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring temp(fd.cFileName);
				listOfFiles.push_back(temp);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

#else

	std::wstring path = folder + L"/*.*";
	std::string pathSTR(path.begin(), path.end());
	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(pathSTR.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				std::string temp1(fd.cFileName);
				std::wstring temp2(temp1.begin(), temp1.end());
				listOfFiles.push_back(temp2);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
#endif
}
