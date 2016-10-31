#include "Game.h"

Game::Game(HINSTANCE hInstance) 
	: DXCore(
	hInstance,		   // The application's handle
	"DirectX Game",			   // Height of the window's client area
	true)			   // Show extra stats (fps) in title bar?
{
	m_entities = FreeVector<vector<SystemBase*>>();
	m_cs = new CollisionSystem();
	m_ts = new TransformSystem();
	// Initialize fields
	//vertexBuffer = 0;
	//indexBuffer = 0;
	/*entities = std::vector<Entity*>();
	vertexShader = 0;
	pixelShader = 0;
	mainCamera = new Camera(XMFLOAT3(0, 0, -10));
	mainCamera->CreateProjectionMatrix(width, height, 80);
	SetCursorPos(width / 2, height / 2);
	prevMousePos.x = 0;
	prevMousePos.y = 0;*/

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

//initialize entity list and systems
void Game::Init() {
	
}

//delete system objects
Game::~Game() {
	delete m_cs;
	delete m_ts;
}

//Advances the game in time
void Game::Update(float dt, float totalTime) {
	m_ts->Update(this, dt);
	m_cs->Update(this, dt);
}

//Removes an entity from all its systems
void Game::RemoveEntity(unsigned int entityId) {
	//bounds check
	if (m_entities.size() > entityId)
	{
		//call remove on the entity ID for each system associated with the entity
		vector<SystemBase*> systems = m_entities[entityId];
		for (SystemBase* s : systems) {
			s->Remove(entityId);
		}
		//free the ID in entity list
		m_entities.free(entityId);
	}
}