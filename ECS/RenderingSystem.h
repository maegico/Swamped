#pragma once
#include "RenderingComponent.h"
#include "ISystem.h"
#include "Vertex.h"
#include "Camera.h"
#include "Lights.h"
#include "FreeVector.h"
#include "EntityIdTypeDef.h"
#include "ClearVector.h"
#include "CollapsedComponent.h"
#include "Particle.h"
#include "ParticleInput.h"
#include "Timeable.h"
#include <d3d11.h>
#include <unordered_map>
#include <map>

struct RenderingComponentData{
	FreeVector<unsigned int> * m_instanceCollection;
	unsigned int m_instanceHandle;
};

class RenderingSystem : public ISystem , public Timeable{
public:
	void Update(Game * game, float dt, float totalTime);
	void Init(Game * game, IDXGISwapChain * swapChain, ID3D11Device * device, ID3D11DeviceContext * context, ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView);
	void Create(EntityId entityId, RenderingComponent * rc);
	void Remove(EntityId enttyId);
	void Collapse();
	RenderingSystem() {};
	~RenderingSystem();

	bool m_fxaaToggle = true;
	bool m_bloomToggle = true;

	Camera						m_camera;
private:
	IDXGISwapChain*				m_swapChain;
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_context;
	ID3D11RenderTargetView*		m_backBufferRTV;
	ID3D11DepthStencilView*		m_depthStencilView;
	unordered_map<RenderingComponent*, FreeVector<ComponentData>>	m_instancedComponents;
	unordered_map<RenderingComponent*, ClearVector<EntityId>> m_collapsedInstancedComponents;
	unordered_map<EntityId, RenderingHandle> m_renderHandles;
	//DirectionalLight		m_dirLights[3];
	Lights						m_lights;

	ParticleMaterial			m_particleMaterial;
	ID3D11BlendState *			m_particleBlendState;

	SkyBoxComponent				m_skyBox;
	ID3D11RasterizerState*		m_skyBoxRasterizerState;
	ID3D11DepthStencilState*	m_skyBoxDepthStencilState;

	// Post process requirements
	//initial render
	ID3D11RenderTargetView*		m_initialRenderRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView*	m_initialRenderSRV;	// Allows us to sample from the same texture

										//bright pixels
	ID3D11RenderTargetView*		m_brightPixelsRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView*	m_brightPixelsSRV;	// Allows us to sample from the same texture

										//blur
	ID3D11RenderTargetView*		m_blurRTV;		// Allows us to render to a texture
	ID3D11ShaderResourceView*	m_blurSRV;	// Allows us to sample from the same texture
	//D3D11_BLEND_DESC		bd;

	ID3D11RenderTargetView*		m_fxaaRTV;
	ID3D11ShaderResourceView*	m_fxaaSRV;

	SimpleVertexShader*			m_fxaaVS;
	SimplePixelShader*			m_fxaaPS;
	ID3D11SamplerState*			m_fxaaSampler;

	ID3D11DepthStencilState*	m_particleDepthStencilState;
};
