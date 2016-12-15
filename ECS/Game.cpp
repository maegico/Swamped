#include "Game.h"
#include "Constructors.h"

Game::Game(HINSTANCE hInstance) 
	: DXCore(
	hInstance,		   // The application's handle
	"DirectX Game",			   // Height of the window's client area
	true)			   // Show extra stats (fps) in title bar?
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

void Game::Init() {
	m_contentManager.Init(m_device, m_context);
	m_renderingSystem.Init(this, m_swapChain, m_device, m_context, m_backBufferRTV, m_depthStencilView);
	Constructors::Init(this);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(53.05f, 0.0f, 0.0f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(-53.05f, 0.0f, 0.0f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(0.0f, 0.0f, 53.05f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(53.05f, 0.0f, 53.05f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(-53.05f, 0.0f, 53.05f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(0.0f, 0.0f, -53.05f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(53.05f, 0.0f, -53.05f), 10.0f);
	Constructors::CreateGround(this, DirectX::XMFLOAT3(-53.05f, 0.0f, -53.05f), 10.0f);
	//m_toggles.push_back(Toggle('A', &m_renderingSystem.m_fxaaToggle));
	m_toggles.push_back(Toggle('B', &m_renderingSystem.m_bloomToggle));
	//border trees
	for (int i = 0; i < 22; i++)
	{
		Constructors::CreateTree(this, DirectX::XMFLOAT3(-75.0f + fRand(-2, 3), 0.0f, -75.0f + (7.0f * i) + fRand(-2, 2)), fRand(1, 3));
	}
	for (int i = 0; i < 22; i++)
	{
		Constructors::CreateTree(this, DirectX::XMFLOAT3(-75.0f + (7.0f * i) + fRand(-2, 2), 0.0f, -75.0f + fRand(-2, 2)), fRand(1, 3));
	}
	for (int i = 0; i < 22; i++)
	{
		Constructors::CreateTree(this, DirectX::XMFLOAT3(-75.0f + (7.0f * i) + fRand(-2, 2), 0.0f, 75.0f + fRand(-2, 2)), fRand(1, 3));
	}
	for (int i = 0; i < 22; i++)
	{
		Constructors::CreateTree(this, DirectX::XMFLOAT3(75.0f + fRand(-2, 2), 0.0f, -75.0f + (7.0f * i) + fRand(-2, 2)), fRand(1, 3));
	}
	//distance trees
	for (int i = 0; i < 7; i++)
	{
		Constructors::CreateTree2(this, DirectX::XMFLOAT3(75.0f + fRand(20, 65), 0.0f, fRand(-75.0f, 75.0f)), fRand(4, 7));
	}
	for (int i = 0; i < 7; i++)
	{
		Constructors::CreateTree2(this, DirectX::XMFLOAT3(-75.0f - fRand(20, 65), 0.0f, fRand(-75.0f, 75.0f)), fRand(4, 7));
	}
	for (int i = 0; i < 7; i++)
	{
		Constructors::CreateTree2(this, DirectX::XMFLOAT3(fRand(-75.0f, 75.0f), 0.0f, -75.0f - fRand(20, 65)), fRand(4, 7));
	}
	/*for (int i = 0; i < 7; i++)
	{
	Constructors::CreateTree(this, DirectX::XMFLOAT3(fRand(-75.0f, 75.0f), 0.0f, 75.0f + fRand(20, 65)), fRand(4, 7));
	}*/
	for (int i = 0; i < 20; i++)
	{
		Constructors::CreateTree2(this, DirectX::XMFLOAT3(15.0f * i + fRand(-3.0f, 3.0f), 0.0f, 75.0f + fRand(20, 65)), fRand(4, 7));
	}

	//rocks
	Constructors::CreateRock(this, DirectX::XMFLOAT3(-5.0f, 0.0f, -1.0f), 4.0f);
	Constructors::CreateRock(this, DirectX::XMFLOAT3(-5.0f, 0.0f, 0.0f), 4.0f);
	for (int i = 0; i < 15; i++)
	{
		Constructors::CreateRock(this, DirectX::XMFLOAT3(fRand(-70, 70), 0.0f, fRand(-70, 70)), fRand(1, 5));
	}

	for (int i = 0; i < 15; i++)
	{
		Constructors::CreateTree2(this, DirectX::XMFLOAT3(fRand(-75, 75), 0.0f, fRand(-75, 75)), fRand(0.5, 4));
	}

	EntityId ghostId = Constructors::CreateGhost(this, DirectX::XMFLOAT3(0.0f, 0.0f, 70.0f), 1.0f);
	m_ghost = Ghost(this, ghostId, m_renderingSystem.m_camera.GetPosition());
	EntityId ghostId2 = Constructors::CreateGhost(this, DirectX::XMFLOAT3(70.0f, 0.0f, 0.0f), 1.0f);
	m_ghost2 = Ghost(this, ghostId2, m_renderingSystem.m_camera.GetPosition());

	m_playerId = Constructors::CreatePlayer(this);
}

//delete system objects
Game::~Game() {
}

//Advances the game in time
void Game::Update(float dt, float totalTime) {
	for (Toggle& t : m_toggles)
		t.Check();

	if (dt > m_timeStep * 4)
		dt = m_timeStep;

	m_accumulator += dt;

	while (m_accumulator >= m_timeStep) {
#if BENCHMARK >= 0
#ifdef _DEBUG
		unsigned int newComponents = 100 * m_timeStep;
#else
		unsigned int newComponents = BENCHMARK * 100 * m_timeStep;
#endif
		for (unsigned int c = 0; c < newComponents; c++)
		{
			Constructors::CreateTestObject(this);
			//Constructors::CreateTestObject2(this);
		}
#endif

		m_transformSystem.Update(this, m_timeStep);
		m_collisionSystem.Update(this, m_timeStep);
		m_ghost.Update(m_renderingSystem.m_camera.GetPosition(),dt);
		m_ghost2.Update(m_renderingSystem.m_camera.GetPosition(),dt);

		auto& playerTransform = m_transformSystem.GetComponent1(m_playerId);
		auto& playerPhysics = m_transformSystem.GetComponent2(m_playerId);
		const float PLAYER_SPEED = 20;
		XMVECTOR direction = XMLoadFloat3(&XMFLOAT3(0, 0, 0));
		//forward and back
		if (GetAsyncKeyState('W') & 0x8000)
		{
			direction = XMVectorAdd(direction, XMLoadFloat3(&XMFLOAT3(0, 0, 1)));
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			direction = XMVectorAdd(direction, XMLoadFloat3(&XMFLOAT3(0, 0, -1)));
		}

		//left and right
		if (GetAsyncKeyState('A') & 0x8000)
		{
			direction = XMVectorAdd(direction, XMLoadFloat3(&XMFLOAT3(-1, 0, 0)));
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			direction = XMVectorAdd(direction, XMLoadFloat3(&XMFLOAT3(1, 0, 0)));
		}

		direction = XMVector3Rotate(XMVector3Normalize(direction), XMQuaternionRotationRollPitchYaw(0,m_playerRotation.y,0));
		XMStoreFloat3(&playerPhysics.m_velocity, XMVectorScale(direction, PLAYER_SPEED));

		XMFLOAT3 cameraPos = XMFLOAT3(playerTransform.m_position.x, 8, playerTransform.m_position.z);
		m_renderingSystem.m_camera.SetPosition(cameraPos);
		m_renderingSystem.m_camera.rotationQuat = playerTransform.m_rotation;

		m_accumulator -= m_timeStep;
	}

	m_particleSystem.Update(this, dt, totalTime);
	m_renderingSystem.Update(this, dt, totalTime);

	//remove all entities queued for removal
	for (unsigned int eId : m_removeQueue) {
		//call remove on the entity ID for each system associated with the entity
		vector<ISystem*> systems = m_entities[eId];
		for (ISystem* s : systems) {
			s->Remove(eId);
		}
		//free the ID in entity list
		m_entities.free(eId);
	}
	m_removeQueue.clear();
}

//Removes an entity from all its systems
void Game::QueueRemoveEntity(EntityId entityId) {
	//bounds check
	if (m_entities.size() > entityId)
	{
		m_removeQueue.add(entityId);
	}
}

void Game::UpdateTitleBarForGame(std::string in) {
	double totalUpdateTime = m_collisionSystem.GetTotalTime() + m_particleSystem.GetTotalTime() + m_renderingSystem.GetTotalTime() + m_transformSystem.GetTotalTime();
	XMFLOAT3 cellCounts = m_collisionSystem.GetCellCounts();
	SetWindowText(hWnd, 
		(in + 
			" Objects: " + std::to_string(m_transformSystem.GetCount()) + 
			" Particles: " + std::to_string(m_particleSystem.GetParticleCount()) + 
			" Cell Divisions: " + std::to_string(static_cast<int>(cellCounts.x)) + 
			" FXAA: "+std::to_string(m_renderingSystem.m_fxaaToggle) + 
			" Bloom: "+std::to_string(m_renderingSystem.m_bloomToggle) + 
			" Collisions: "+std::to_string((m_collisionSystem.GetTotalTime()/totalUpdateTime)) +
			" Particles: " + std::to_string((m_particleSystem.GetTotalTime() / totalUpdateTime)) +
			" Rendering: " + std::to_string((m_renderingSystem.GetTotalTime() / totalUpdateTime)) +
			" Transforms: " + std::to_string((m_transformSystem.GetTotalTime() / totalUpdateTime))
		).c_str());
}

void Game::OnResize() {
	DXCore::OnResize();
	m_renderingSystem.OnResize(this);
}

void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{

	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{

	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	auto& playerTransform = m_transformSystem.GetComponent1(m_playerId);
	//static int lastPos[2] = { x,y };
	if (buttonState & 0x0002)
	{
		//XMStoreFloat4(&playerTransform.m_rotation, XMQuaternionMultiply(XMQuaternionMultiply(XMQuaternionRotationRollPitchYaw(0, XM_PI*( y - prevMousePos.y)/180, 0), XMLoadFloat4(&playerTransform.m_rotation)),XMQuaternionRotationRollPitchYaw(0,0, XM_PI*(x - prevMousePos.x) / 180)));
		  //(x - prevMousePos.x), (y - prevMousePos.y);
		m_playerRotation.x += .3*XM_PI*(y - prevMousePos.y) / 180;
		m_playerRotation.y += .3*XM_PI*(x - prevMousePos.x) / 180;
		XMStoreFloat4(&playerTransform.m_rotation, XMQuaternionRotationRollPitchYaw(m_playerRotation.x, m_playerRotation.y, 0));
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}