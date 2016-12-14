#include "RenderingSystem.h"
#include "Game.h"

RenderingSystem::~RenderingSystem() {
	m_particleBlendState->Release();
	m_skyBoxRasterizerState->Release();
	m_skyBoxDepthStencilState->Release();

	m_initialRenderRTV->Release();
	m_initialRenderSRV->Release();

	m_brightPixelsRTV->Release();
	m_brightPixelsSRV->Release();

	m_blurRTV->Release(); 
	m_blurSRV->Release();	

	m_fxaaRTV->Release();
	m_fxaaSRV->Release();

	m_particleDepthStencilState->Release();
}

//Initializes the rendering system
void RenderingSystem::Init(Game * game, IDXGISwapChain * swapChain, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView) {
	D3D11_BLEND_DESC bd = {};
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	bd.IndependentBlendEnable = true;
	bd.AlphaToCoverageEnable = true;

	HRESULT blendStateCreate = device->CreateBlendState(&bd, &m_particleBlendState);
	
	m_camera = Camera(XMFLOAT3(0, 20, 0));
	m_camera.RotationDelta(-.5, 0);
	m_camera.CreateProjectionMatrix(1920, 1080, 103);
	m_swapChain = swapChain;
	m_device = device;
	m_context = context;
	m_backBufferRTV = renderTargetView;
	m_depthStencilView = depthStencilView;

	DirectionalLight dirLight1 = { XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f),
		XMFLOAT3(1.0f, -1.0f, 0.0f) };

	PointLight pointLight = { XMFLOAT4(0.4f, 0.6f, 0.4f, 1.0f),
		XMFLOAT3(1.0f, -1.0f, 0.0f),
		2.0f };

	SpotLight spotLight = { XMFLOAT4(0.4f, 0.6f, 0.8f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		2.0f,
		XMFLOAT3(-1.0f, 0.0f, 0.0f) };

	m_lights = { { spotLight },
	{ pointLight },
		dirLight1,
		XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f),
		1,
		1 };

	m_particleMaterial = game->m_contentManager.GetParticleMaterial("snowflake");
	m_skyBox = { game->m_contentManager.GetSkyBoxMaterial("skyMap"), game->m_contentManager.GetMeshStore("cube.obj").m_m };

	// Create a rasterizer state so we can render backfaces
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &m_skyBoxRasterizerState);

	// Create a depth state so that we can accept pixels
	// at a depth less than or EQUAL TO an existing depth
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Make sure we can see the sky (at max depth)
	device->CreateDepthStencilState(&dsDesc, &m_skyBoxDepthStencilState);

	//Post process resources
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = game->GetWidth();
	textureDesc.Height = game->GetHeight();
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;


	ID3D11Texture2D* initialRender;
	ID3D11Texture2D* brightPixels;
	ID3D11Texture2D* blur;
	ID3D11Texture2D* fxaa;
	device->CreateTexture2D(&textureDesc, 0, &initialRender);
	device->CreateTexture2D(&textureDesc, 0, &brightPixels);
	device->CreateTexture2D(&textureDesc, 0, &blur);
	device->CreateTexture2D(&textureDesc, 0, &fxaa);

	device->CreateRenderTargetView(initialRender, &rtvDesc, &m_initialRenderRTV);
	device->CreateRenderTargetView(brightPixels, &rtvDesc, &m_brightPixelsRTV);
	device->CreateRenderTargetView(blur, &rtvDesc, &m_blurRTV);
	device->CreateRenderTargetView(fxaa, &rtvDesc, &m_fxaaRTV);

	device->CreateShaderResourceView(initialRender, &srvDesc, &m_initialRenderSRV);
	device->CreateShaderResourceView(brightPixels, &srvDesc, &m_brightPixelsSRV);
	device->CreateShaderResourceView(blur, &srvDesc, &m_blurSRV);
	device->CreateShaderResourceView(fxaa, &srvDesc, &m_fxaaSRV);

	initialRender->Release();
	brightPixels->Release();
	blur->Release();
	fxaa->Release();

	m_fxaaVS = game->m_contentManager.GetVShader("FxaaVS.cso");
	m_fxaaPS = game->m_contentManager.GetPShader("FxaaPS.cso");
	game->m_contentManager.GetSampler("fxaaSampler");

	D3D11_DEPTH_STENCIL_DESC pdsDesc = {};

	//Depth test parameters
	pdsDesc.DepthEnable = true;
	pdsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pdsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	m_device->CreateDepthStencilState(&pdsDesc, &m_particleDepthStencilState);
}

//Create a rendering component
void RenderingSystem::Create(EntityId entityId, RenderingComponent * rc) {
	//get collection in map
	FreeVector<ComponentData> * collection = &(m_instancedComponents[rc]);
	//create render handle and add to collection
	m_renderHandles[entityId] = { collection, collection->add({true, entityId}) };
}

//Remove a rendering component
void RenderingSystem::Remove(EntityId entityId) {
	//get handle
	RenderingHandle rh = m_renderHandles[entityId];
	//de-activate and remove from collection if handle is valid (it won't be on a double remove)
	if (rh.m_rcList != nullptr)
	{
		(*rh.m_rcList)[rh.m_index].m_active = false;
		rh.m_rcList->free(rh.m_index);
	}
	//remove handle
	m_renderHandles.erase(entityId);
}

//Copies all active components into the collapsed component list
void RenderingSystem::Collapse() {
	//clear collapsed collections
	for (auto& rcp : m_collapsedInstancedComponents) {
		rcp.second.clear();
	}
	//iterate through each collection in the component map
	for (auto& rcp : m_instancedComponents) {
		FreeVector<ComponentData> & collection = rcp.second;
		for (unsigned int c = 0; c < collection.size(); c++)
		{
			//if a component in the collection is active, copy its entityID to the corresponding collapsed collection
			if (collection[c].m_active)
				m_collapsedInstancedComponents[rcp.first].add(collection[c].m_entityId);
		}
	}
}

//Draws all the stuff
void RenderingSystem::Update(Game * game, float dt, float totalTime) {
	StartTimer();
	Collapse();
	m_camera.Update();

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	m_context->ClearRenderTargetView(m_initialRenderRTV, color);
	m_context->ClearRenderTargetView(m_brightPixelsRTV, color);
	m_context->ClearRenderTargetView(m_blurRTV, color);
	m_context->ClearRenderTargetView(m_backBufferRTV, color);
	m_context->ClearDepthStencilView(
		m_depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	if(m_bloomToggle)
		m_context->OMSetRenderTargets(1, &m_initialRenderRTV, m_depthStencilView);
	else if(m_fxaaToggle)
		m_context->OMSetRenderTargets(1, &m_fxaaRTV, m_depthStencilView);
	else
		m_context->OMSetRenderTargets(1, &m_backBufferRTV, m_depthStencilView);

	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//for each mesh/material combination
	for (auto& rcp : m_collapsedInstancedComponents)
	{
		//assemble list of world matrices
		RenderingComponent rc = *rcp.first;
		ClearVector<EntityId> & collection = rcp.second;
		vector<XMFLOAT4X4> worldMatrices;
		worldMatrices.resize(collection.size());
		parallel_for(size_t(0), collection.size(),[&](unsigned int c){
			TransformComponent tc = game->m_transformSystem.GetComponent1(collection[c]);
			XMStoreFloat4x4(&worldMatrices[c], XMMatrixTranspose(TransformSystem::GetMatrix(tc)));
		});

		//create buffer for world matrices
		D3D11_BUFFER_DESC instDesc = {};
		instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instDesc.ByteWidth = sizeof(XMFLOAT4X4) * collection.size();
		instDesc.CPUAccessFlags = 0;
		instDesc.MiscFlags = 0;
		instDesc.StructureByteStride = 0;
		instDesc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA transforms = {};
		transforms.pSysMem = &worldMatrices[0];

		ID3D11Buffer * instanceBuffer;
		m_device->CreateBuffer(&instDesc, &transforms, &instanceBuffer);

		//get shaders
		SimpleVertexShader * vertexShader = rc.m_material.vertexShader;
		SimplePixelShader * pixelShader = rc.m_material.pixelShader;
		
		//set and copy data
		vertexShader->SetMatrix4x4("view", m_camera.GetView());
		vertexShader->SetMatrix4x4("projection", m_camera.GetProjection());
		vertexShader->SetFloat3("cameraPos", m_camera.GetPosition());
		vertexShader->SetFloat("fogHeight", 20);

		vertexShader->CopyAllBufferData();

		//pixelShader->SetData("dirLight1", &m_dirLights[0], sizeof(DirectionalLight));
		//pixelShader->SetData("dirLight2", &m_dirLights[1], sizeof(DirectionalLight));
		//pixelShader->SetData("dirLight3", &m_dirLights[2], sizeof(DirectionalLight));
		pixelShader->SetData("lights", &m_lights, sizeof(Lights));
		pixelShader->SetShaderResourceView("Texture", rc.m_material.textureView);
		pixelShader->SetShaderResourceView("NormalMap", rc.m_material.normalMap);
		pixelShader->SetSamplerState("Sampler", rc.m_material.samplerState);

		pixelShader->CopyAllBufferData();

		//set shaders
		vertexShader->SetShader();
		pixelShader->SetShader();

		Mesh& m = rc.m_mesh;
		ID3D11Buffer* vbs[2] = {
			m.vertexBuffer ,	// Per-vertex data
			instanceBuffer		// Per-instance data
		};

		// Two buffers means two strides and two offsets!
		UINT strides[2] = { sizeof(Vertex), sizeof(XMFLOAT4X4) };
		UINT offsets[2] = { 0, 0 };

		// Set both vertex buffers
		m_context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
		m_context->IASetIndexBuffer(m.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//draw
		m_context->DrawIndexedInstanced(
			m.indexCount,		// Number of indices from index buffer
			collection.size(),	// Number of instances to actually draw
			0, 0, 0);

		instanceBuffer->Release();
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_context->IASetVertexBuffers(0, 1, &m_skyBox.m_mesh.vertexBuffer, &stride, &offset);
	m_context->IASetIndexBuffer(m_skyBox.m_mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	SimplePixelShader* skyPS = m_skyBox.m_material.pixelShader;
	SimpleVertexShader* skyVS = m_skyBox.m_material.vertexShader;
	ID3D11ShaderResourceView* skySRV = m_skyBox.m_material.textureView;

	// Set up shaders
	skyVS->SetMatrix4x4("view", m_camera.GetView());
	skyVS->SetMatrix4x4("projection", m_camera.GetProjection());
	skyVS->CopyAllBufferData();
	skyVS->SetShader();

	skyPS->SetShaderResourceView("Sky", skySRV);
	skyPS->SetSamplerState("Sampler", m_skyBox.m_material.samplerState);
	skyPS->CopyAllBufferData();
	skyPS->SetShader();

	// Set the proper render states
	m_context->RSSetState(m_skyBoxRasterizerState);
	m_context->OMSetDepthStencilState(m_skyBoxDepthStencilState, 0);

	// Actually draw
	m_context->DrawIndexed(m_skyBox.m_mesh.indexCount, 0, 0);

	// Reset the states!
	m_context->RSSetState(0);
	m_context->OMSetDepthStencilState(0, 0);

	vector<Particle> & particles = game->m_particleSystem.GetParticles();
	unsigned int particleCount = game->m_particleSystem.GetParticleCount();

	if (particleCount > 0) {

		m_particleMaterial.vertexShader->SetShader();
		m_particleMaterial.vertexShader->SetFloat("currentTime", totalTime);
		m_particleMaterial.geometryShader->SetShader();
		m_particleMaterial.geometryShader->SetMatrix4x4("view", m_camera.GetView());
		m_particleMaterial.geometryShader->SetMatrix4x4("projection", m_camera.GetProjection());
		m_particleMaterial.geometryShader->SetData("cameraPos", &m_camera.GetPosition(), sizeof(XMFLOAT3));
		m_particleMaterial.pixelShader->SetShader();
		m_particleMaterial.pixelShader->SetShaderResourceView("pTexture", m_particleMaterial.textureView);
		m_particleMaterial.pixelShader->SetSamplerState("pSampler", m_particleMaterial.samplerState);

		m_particleMaterial.vertexShader->CopyAllBufferData();
		m_particleMaterial.geometryShader->CopyAllBufferData();
		m_particleMaterial.pixelShader->CopyAllBufferData();

		m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		//create buffer for world matrices
		D3D11_BUFFER_DESC partDesc = {};
		partDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		partDesc.ByteWidth = sizeof(Particle) * particleCount;
		partDesc.CPUAccessFlags = 0;
		partDesc.MiscFlags = 0;
		partDesc.StructureByteStride = 0;
		partDesc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA particleStructs = {};
		particleStructs.pSysMem = &particles[0];

		ID3D11Buffer * particleBuffer;
		m_device->CreateBuffer(&partDesc, &particleStructs, &particleBuffer);

		UINT stride = sizeof(Particle);
		UINT offset = 0;

		m_context->IASetVertexBuffers(0, 1, &particleBuffer, &stride, &offset);
		m_context->OMSetBlendState(m_particleBlendState, 0, 0xffffffff);
		m_context->OMSetDepthStencilState(m_particleDepthStencilState, 0);
		//m_context->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);

		//m_context->RSSetState()

		m_context->Draw(particleCount, 0);

		particleBuffer->Release();

		m_context->GSSetShader(NULL, 0, 0);
		m_context->OMSetBlendState(NULL, 0, 0xffffffff);
		m_context->OMSetDepthStencilState(NULL, 0);
		m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	ID3D11ShaderResourceView* initialReset = { NULL };

	if (m_bloomToggle) {
		//Post Processing
		m_context->OMSetRenderTargets(1, &m_brightPixelsRTV, 0);

		//Set the Shaders
		//Send data to the Shaders
		//copy all the buffer data

		Material brightPixels = game->m_contentManager.GetMaterial("BrightPixels");
		SimplePixelShader* brightPixelsPS = brightPixels.pixelShader;

		brightPixels.vertexShader->SetShader();
		brightPixelsPS->SetShader();
		brightPixelsPS->SetShaderResourceView("InitialRender", m_initialRenderSRV);
		brightPixelsPS->SetSamplerState("Sampler", brightPixels.samplerState);
		brightPixelsPS->SetFloat("brightnessMin", .2);
		brightPixelsPS->SetFloat("brightnessMax", .5);
		brightPixelsPS->SetFloat("exponent", 1.5);
		brightPixelsPS->CopyAllBufferData();

		//then draw
		//ID3D11Buffer* nothing1 = 0;
		ID3D11Buffer* nothing = 0;
		m_context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
		m_context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

		////draw the triangle that encompasses the whole screen
		m_context->Draw(3, 0);

		//unbind initialRender from brightPixelPS so we can write to it next frame
		m_context->PSSetShaderResources(0, 1, &initialReset);

		m_context->OMSetRenderTargets(1, &m_blurRTV, 0);

		Material blurMat = game->m_contentManager.GetMaterial("Blur");
		SimplePixelShader* blurPS = blurMat.pixelShader;

		blurMat.vertexShader->SetShader();

		blurPS->SetShader();

		blurPS->SetShaderResourceView("BrightPixels", m_brightPixelsSRV);
		blurPS->SetSamplerState("Sampler", blurMat.samplerState);
		blurPS->SetInt("blurAmount", 5);
		blurPS->SetFloat("pixelWidth", 1.0f / game->GetWidth());
		blurPS->SetFloat("pixelHeight", 1.0f / game->GetHeight());
		blurPS->CopyAllBufferData();

		//then draw
		//ID3D11Buffer* nothing2 = 0;
		m_context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
		m_context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

		//draw the triangle that encompasses the whole screen
		m_context->Draw(3, 0);

		//brightPixelsPS->SetShaderResourceView("InitialRender", 0);
		//blurPS->SetShaderResourceView("Blur", 0);


		//the last pass
		m_context->OMSetRenderTargets(1, (m_fxaaToggle) ? &m_fxaaRTV : &m_backBufferRTV, 0);

		Material bloom = game->m_contentManager.GetMaterial("Bloom");
		SimplePixelShader* bloomPS = bloom.pixelShader;

		bloom.vertexShader->SetShader();
		bloomPS->SetShader();
		bloomPS->SetShaderResourceView("InitialRender", m_initialRenderSRV);
		bloomPS->SetShaderResourceView("Blur", m_blurSRV);
		bloomPS->SetSamplerState("Sampler", bloom.samplerState);
		bloomPS->CopyAllBufferData();

		//then draw
		//ID3D11Buffer* nothing3 = 0;
		m_context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
		m_context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

		////draw the triangle that encompasses the whole screen
		m_context->Draw(3, 0);

		//unbind initialRender from bloomPS so we can write to it next frame
		m_context->PSSetShaderResources(0, 1, &initialReset);
	}

	if (m_fxaaToggle) {

		m_context->OMSetRenderTargets(1, &m_backBufferRTV, 0);

		//XMFLOAT4 rcpFrame = XMFLOAT4(1.0f / game->GetWidth(), 1.0f / game->GetHeight(), 0.0f, 0.0f);
		m_fxaaVS->SetShader();
		m_fxaaPS->SetShader();
		m_fxaaPS->SetFloat4("rcpFrame", XMFLOAT4(1.0f / game->GetWidth(), 1.0f / game->GetHeight(), 0.0f, 0.0f));
		m_fxaaPS->SetSamplerState("anisotropicSampler", m_fxaaSampler);
		m_fxaaPS->SetShaderResourceView("inputTexture", m_fxaaSRV);
		m_fxaaPS->CopyAllBufferData();

		m_context->Draw(3, 0);

		//unbind initialRender from bloomPS so we can write to it next frame
		m_context->PSSetShaderResources(0, 1, &initialReset);
	}

	// Present the back buffer to the user
	m_swapChain->Present(0, 0);

	StopTimer();
}