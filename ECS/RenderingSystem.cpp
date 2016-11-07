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
	unsigned int transformIndex = 0;
	for (unsigned int c = 0; c < m_collapsedCount; c++) {

		RenderingComponent& rc = m_collapsedComponents[c].m_component;

		SimpleVertexShader * vertexShader = rc.m_material.vertexShader;
		SimplePixelShader * pixelShader = rc.m_material.pixelShader;
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		DirectX::XMFLOAT4X4 wm;
		g->m_ts.SearchForEntityId(transformIndex, m_collapsedComponents[c].m_entityId);
		XMStoreFloat4x4(&wm, XMMatrixTranspose(TransformSystem::GetMatrix(g->m_ts.GetComponentList1()[transformIndex])));
		vertexShader->SetMatrix4x4("world", wm);
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
		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer * vb = m.vertexBuffer;
		m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_context->IASetIndexBuffer(m.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		m_context->DrawIndexed(
			m.indexCount,     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	m_swapChain->Present(0, 0);
}