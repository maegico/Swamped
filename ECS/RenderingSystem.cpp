#include "RenderingSystem.h"
#include "Game.h"

//Initializes the rendering system
void RenderingSystem::Init(IDXGISwapChain * swapChain, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView) {
	m_camera = Camera(XMFLOAT3(0, 5, -50));
	m_camera.RotationDelta(.5, 0);
	m_camera.CreateProjectionMatrix(1920, 1080, 90);
	m_swapChain = swapChain;
	m_device = device;
	m_context = context;
	m_backBufferRTV = renderTargetView;
	m_depthStencilView = depthStencilView;
	m_dirLights[0] = { {1,0,0,1},{.1f,0,0,1},{0,1,1} };
	m_dirLights[1] = { { 0,1,0,1 },{ .1f,0,0,1 },{ 1,1,0 } };
	m_dirLights[2] = { { 0,0,1,1 },{ .1f,0,0,1 },{ 1,0,1 } };
	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
void RenderingSystem::Update(Game * g, float dt) {
	Collapse();
	m_camera.Update();

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	m_context->ClearRenderTargetView(m_backBufferRTV, color);
	m_context->ClearDepthStencilView(
		m_depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//for each mesh/material combination
	for (auto& rcp : m_collapsedInstancedComponents)
	{
		//assemble list of world matrices
		RenderingComponent rc = *rcp.first;
		ClearVector<EntityId> & collection = rcp.second;
		vector<XMFLOAT4X4> worldMatrices;
		worldMatrices.resize(collection.size());
		parallel_for(size_t(0), collection.size(),[&](unsigned int c){
			TransformComponent tc = g->m_ts.GetComponent1(collection[c]);
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

		vertexShader->CopyAllBufferData();

		pixelShader->SetData("dirLight1", &m_dirLights[0], sizeof(DirectionalLight));
		pixelShader->SetData("dirLight2", &m_dirLights[1], sizeof(DirectionalLight));
		pixelShader->SetData("dirLight3", &m_dirLights[2], sizeof(DirectionalLight));
		pixelShader->SetShaderResourceView("diffuseTexture", rc.m_material.textureView);
		pixelShader->SetSamplerState("basicSampler", rc.m_material.samplerState);

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

	// Present the back buffer to the user
	m_swapChain->Present(0, 0);
}