#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(IDXGISwapChain * swapChain, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView) {
	m_swapChain = swapChain;
	m_device = device;
	m_context = context;
	m_backBufferRTV = renderTargetView;
	m_depthStencilView = depthStencilView;
}

void RenderingSystem::Update(Game * g, float dt) {
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

	/*for (Entity* e : entities)
	{
		e->PrepareMaterial(m_camera->GetView(), m_camera->GetProjection());
		Mesh *m = e->GetMesh();
		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer * vb = m->GetVertexBuffer();
		m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_context->IASetIndexBuffer(m->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		m_context->DrawIndexed(
			e->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}*/

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	m_swapChain->Present(0, 0);
}