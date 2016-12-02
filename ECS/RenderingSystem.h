#pragma once
#include "RenderingComponent.h"
#include "ISystem.h"
#include "Vertex.h"
#include "Camera.h"
#include "Lights.h"
#include "FreeVector.h"
#include "EntityIdTypeDef.h"
#include "ClearVector.h"
#include <d3d11.h>
#include <unordered_map>
#include <map>

struct RenderingComponentData{
	FreeVector<unsigned int> * m_instanceCollection;
	unsigned int m_instanceHandle;
};

class RenderingSystem : public ISystem {
public:
	void Update(Game * game, float dt);
	void Init(IDXGISwapChain * swapChain, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView);
	void Create(EntityId entityId, RenderingComponent * rc);
	void Remove(EntityId enttyId);
	void Collapse();
	RenderingSystem() {};
private:
	IDXGISwapChain*			m_swapChain;
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_context;
	ID3D11RenderTargetView* m_backBufferRTV;
	ID3D11DepthStencilView* m_depthStencilView;
	Camera					m_camera;
	unordered_map<RenderingComponent*, FreeVector<ComponentData>>	m_instancedComponents;
	unordered_map<RenderingComponent*, ClearVector<EntityId>> m_collapsedInstancedComponents;
	unordered_map<EntityId, RenderingHandle> m_renderHandles;
	//DirectionalLight		m_dirLights[3];
	Lights					m_lights;
};
