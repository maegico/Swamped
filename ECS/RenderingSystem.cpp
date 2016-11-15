#include "RenderingSystem.h"
#include "Game.h"

void RenderingSystem::Init(IDXGISwapChain * swapChain, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView) {
	m_camera = Camera(XMFLOAT3(0, 5, -50));
	m_camera.RotationDelta(.5, 0);
	m_camera.CreateProjectionMatrix(1920, 1080, 103);
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

void RenderingSystem::Create(EntityId entityId, RenderingComponent * rc) {
	FreeVector<ComponentData> * collection = &(m_instancedComponents[rc]);
	m_renderHandles[entityId] = { collection, collection->add(ComponentData(entityId)) };
	//unsigned int index = Create(entityId, {});
	//FreeVector<unsigned int> * collection = &m_instancedComponents[rc];
	//collection->add(entityId);
	//m_instancedComponentData[index] = { collection, collection->add(entityId) };
}

void RenderingSystem::Remove(EntityId entityId) {
	RenderingHandle rh = m_renderHandles[entityId];
	if (rh.m_rcList != nullptr)
	{
		(*rh.m_rcList)[rh.m_index].m_active = false;
		rh.m_rcList->free(rh.m_index);
	}
	m_renderHandles.erase(entityId);
}

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
				m_collapsedInstancedComponents[rcp.first].add(collection[c].GetEntityId());
		}
	}
}

void RenderingSystem::Update(Game * g, float dt) {
	Collapse();
	m_camera.Update();
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	m_context->ClearRenderTargetView(m_backBufferRTV, color);
	m_context->ClearDepthStencilView(
		m_depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	//unsigned int transformIndex = 0;
	//for (unsigned int c = 0; c < m_collapsedCount; c++) {
	//	RenderingComponent& rc = m_collapsedComponents[c].m_component;
	//	if (rc.m_material.vertexShader == nullptr)
	//		continue;
	//	SimpleVertexShader * vertexShader = rc.m_material.vertexShader;
	//	SimplePixelShader * pixelShader = rc.m_material.pixelShader;
	//	// Send data to shader variables
	//	//  - Do this ONCE PER OBJECT you're drawing
	//	//  - This is actually a complex process of copying data to a local buffer
	//	//    and then copying that entire buffer to the GPU.  
	//	//  - The "SimpleShader" class handles all of that for you.
	//	DirectX::XMFLOAT4X4 wm;
	//	g->m_ts.SearchForEntityId(transformIndex, m_collapsedComponents[c].m_entityId);
	//	XMStoreFloat4x4(&wm, XMMatrixTranspose(TransformSystem::GetMatrix(g->m_ts.GetComponentList1()[transformIndex])));
	//	vertexShader->SetMatrix4x4("world", wm);
	//	vertexShader->SetMatrix4x4("view", m_camera.GetView());
	//	vertexShader->SetMatrix4x4("projection", m_camera.GetProjection());

	//	// Once you've set all of the data you care to change for
	//	// the next draw call, you need to actually send it to the GPU
	//	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	//	vertexShader->CopyAllBufferData();

	//	pixelShader->SetData("dirLight1", &m_dirLights[0], sizeof(DirectionalLight));
	//	pixelShader->SetData("dirLight2", &m_dirLights[1], sizeof(DirectionalLight));
	//	pixelShader->SetData("dirLight3", &m_dirLights[2], sizeof(DirectionalLight));
	//	pixelShader->SetShaderResourceView("diffuseTexture", rc.m_material.textureView);
	//	pixelShader->SetSamplerState("basicSampler", rc.m_material.samplerState);
	//	pixelShader->CopyAllBufferData();

	//	// Set the vertex and pixel shaders to use for the next Draw() command
	//	//  - These don't technically need to be set every frame...YET
	//	//  - Once you start applying different shaders to different objects,
	//	//    you'll need to swap the current shaders before each draw
	//	vertexShader->SetShader();
	//	pixelShader->SetShader();

	//	Mesh& m = rc.m_mesh;
	//	// Set buffers in the input assembler
	//	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//	//    have different geometry.
	//	UINT stride = sizeof(Vertex);
	//	UINT offset = 0;
	//	ID3D11Buffer * vb = m.vertexBuffer;
	//	m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	//	m_context->IASetIndexBuffer(m.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//	// Finally do the actual drawing
	//	//  - Do this ONCE PER OBJECT you intend to draw
	//	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//	//     vertices in the currently set VERTEX BUFFER
	//	m_context->DrawIndexed(
	//		m.indexCount,     // The number of indices to use (we could draw a subset if we wanted)
	//		0,     // Offset to the first index we want to use
	//		0);    // Offset to add to each index when looking up vertices
	//}

	for (auto& rcp : m_collapsedInstancedComponents)
	{
		RenderingComponent rc = *rcp.first;
		ClearVector<EntityId> & collection = rcp.second;
		vector<XMFLOAT4X4> worldMatrices;
		worldMatrices.resize(collection.size());
		parallel_for(size_t(0), collection.size(),[&](unsigned int c){
			TransformComponent tc = g->m_ts.GetComponent1(collection[c]);
			XMStoreFloat4x4(&worldMatrices[c], XMMatrixTranspose(TransformSystem::GetMatrix(tc)));
		});

		/*D3D11_BUFFER_DESC instDesc = {};
		instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instDesc.ByteWidth = sizeof(XMFLOAT4X4) * collection.size();
		instDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		instDesc.MiscFlags = 0;
		instDesc.StructureByteStride = 0;
		instDesc.Usage = D3D11_USAGE_DYNAMIC;*/
		D3D11_BUFFER_DESC instDesc = {};
		instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instDesc.ByteWidth = sizeof(XMFLOAT4X4) * collection.size();
		instDesc.CPUAccessFlags = 0;
		instDesc.MiscFlags = 0;
		instDesc.StructureByteStride = 0;
		instDesc.Usage = D3D11_USAGE_IMMUTABLE;

		// Map the resource, so we get an address for it
		// This lets us memcpy directly to it
		D3D11_SUBRESOURCE_DATA transforms = {};
		transforms.pSysMem = &worldMatrices[0];

		ID3D11Buffer * instanceBuffer;
		m_device->CreateBuffer(&instDesc, &transforms, &instanceBuffer);

		SimpleVertexShader * vertexShader = rc.m_material.vertexShader;
		SimplePixelShader * pixelShader = rc.m_material.pixelShader;
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		//DirectX::XMFLOAT4X4 wm;
		//g->m_ts.SearchForEntityId(transformIndex, m_collapsedComponents[c].m_entityId);
		//XMStoreFloat4x4(&wm, XMMatrixTranspose(TransformSystem::GetMatrix(g->m_ts.GetComponentList1()[transformIndex])));
		//vertexShader->SetMatrix4x4("world", wm);
		vertexShader->SetMatrix4x4("view", m_camera.GetView());
		vertexShader->SetMatrix4x4("projection", m_camera.GetProjection());

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();

		pixelShader->SetData("dirLight1", &m_dirLights[0], sizeof(DirectionalLight));
		pixelShader->SetData("dirLight2", &m_dirLights[1], sizeof(DirectionalLight));
		pixelShader->SetData("dirLight3", &m_dirLights[2], sizeof(DirectionalLight));
		pixelShader->SetShaderResourceView("diffuseTexture", rc.m_material.textureView);
		pixelShader->SetSamplerState("basicSampler", rc.m_material.samplerState);
		pixelShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		vertexShader->SetShader();
		pixelShader->SetShader();

		Mesh& m = rc.m_mesh;
		ID3D11Buffer* vbs[2] = {
			m.vertexBuffer ,	// Per-vertex data
			instanceBuffer			// Per-instance data
		};

		// Two buffers means two strides and two offsets!
		UINT strides[2] = { sizeof(Vertex), sizeof(XMFLOAT4X4) };
		UINT offsets[2] = { 0, 0 };

		// Set both vertex buffers
		m_context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
		m_context->IASetIndexBuffer(m.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		m_context->DrawIndexedInstanced(
			m.indexCount, // Number of indices from index buffer
			collection.size(),					// Number of instances to actually draw
			0, 0, 0);

		instanceBuffer->Release();
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	m_swapChain->Present(0, 0);
}