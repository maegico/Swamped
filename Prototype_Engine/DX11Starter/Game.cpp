#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	delete camera;

	for (int i = 0; i < entities.size(); i++)
		delete entities[i];

	delete cmanager;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	dirLight1 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
				XMFLOAT3(1.0f, -1.0f, 0.0f) };

	dirLight2 = { XMFLOAT4(0.3f, 0.1f, 0.2f, 1.0f),
				XMFLOAT4(0.5f, 1.0f, 0.32f, 1.0f),
				XMFLOAT3(1.0f, -1.0f, 0.0f) };

	dirLight3 = { XMFLOAT4(0.3f, 0.1f, 0.2f, 1.0f),
				XMFLOAT4(0.5f, 1.0f, 0.32f, 1.0f),
				XMFLOAT3(-1.0f, 1.0f, 1.0f) };


	camera = new Camera();

	//cmanager = new ContentManager(device, context);
	cmanager = new ContentManager();
	cmanager->Init(device, context);
	Material* mat1 = cmanager->LoadMaterial("soilDirLight", "sampler", "VertexShader.cso", "PixelShader.cso", "soilrough.png");
	Material* mat2 = cmanager->LoadMaterial("styroDirLight", "sampler", "VertexShader.cso", "PixelShader.cso", "styrofoam.png");

	entities = {
		new Entity(context, cmanager->GetMesh("cube.obj"), cmanager->GetMaterial("soilDirLight"),{ 0.5, 0.5, 0 }, 0),
		new Entity(context, cmanager->GetMesh("cone.obj"), cmanager->GetMaterial("styroDirLight"),{ 0, 0, 0 }, 0),
		new Entity(context, cmanager->GetMesh("helix.obj"), cmanager->GetMaterial("soilDirLight"),{ -1.0f, 0, 0 }, 0),
		new Entity(context, cmanager->GetMesh("cube.obj"), cmanager->GetMaterial("styroDirLight"),{ 0.0f, -0.5f, 0 }, 0),
		new Entity(context, cmanager->GetMesh("avent.obj"), cmanager->GetMaterial("soilDirLight"),{ 1.0f, -1.0f, 0 }, 0) };

	ID3D11Buffer* test = entities[0]->getMesh()->GetVertexBuffer();

	camera->updateProjection(width, height);
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->updateProjection(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	camera->update(deltaTime);

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->move({ 0.25f * deltaTime, 0.0f * deltaTime, 0.0f * deltaTime });
		entities[i]->update(deltaTime);
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//// Send data to shader variables
	////  - Do this ONCE PER OBJECT you're drawing
	////  - This is actually a complex process of copying data to a local buffer
	////    and then copying that entire buffer to the GPU.  
	////  - The "SimpleShader" class handles all of that for you.
	
	for (int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];
		Material* mat = entity->getMat();
		ID3D11SamplerState* sampler = mat->getSampler();
		ID3D11ShaderResourceView* shaderResView = mat->getShaderResView();

		mat->getVShader()->SetMatrix4x4("view", camera->getViewMatrix());
		mat->getVShader()->SetMatrix4x4("projection", camera->getProjectionMatrix());
		mat->getPShader()->SetData("dirLight1", &dirLight1, sizeof(DirectionalLight));
		mat->getPShader()->SetData("dirLight2", &dirLight2, sizeof(DirectionalLight));
		mat->getPShader()->SetData("dirLight3", &dirLight3, sizeof(DirectionalLight));
		mat->getPShader()->SetSamplerState("basicSampler", sampler);
		mat->getPShader()->SetShaderResourceView("diffuseTexture", shaderResView);
	
		entity->draw();

		//did this as a nice way to use the material
		mat->ReleaseSampler();
		mat->ReleaseShaderResView();
		//sampler->Release();
		//shaderResView->Release();
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	moving = true;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	moving = false;

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	if (moving)
	{
		// Add any custom code here...
		camera->addRotX(prevMousePos.y - y);
		camera->addRotY(prevMousePos.x - x);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion