//-----------------------------------------------------------------------------
// File: Framework.cpp
// Description : This Class managers the Framework
// Author : Ian Wigley
//-----------------------------------------------------------------------------

#include "Framework.h"
#include "Tank.h"
#include "SkyDome.h"
#include "FrameWorkTest.h"
#include "FrameWorkResourceManager.h"

Framework* _frame =  NULL;
FrameWorkTest* _frameWorkTest;
FrameWorkResourceManager* _frameWorkResourceManager = NULL;

//-----------------------------------------------------------------------------
// Name: Framework()
// Desc: FrameWork Class Constructor
//-----------------------------------------------------------------------------
Framework::Framework(void)
{
}

//-----------------------------------------------------------------------------
// Name: ~Framework()
// Desc: FrameWork Class Destructor
//-----------------------------------------------------------------------------
Framework::~Framework(void)
{
	_frameWorkResourceManager->Shutdown();
	Shutdown();
}

//-----------------------------------------------------------------------------
// Name: Framework()
// Desc: FrameWork Class Constructor
//-----------------------------------------------------------------------------
Framework::Framework(HINSTANCE hInstance)
{
	_pD3D = NULL;
	_pd3dDevice = NULL;
	_pMesh = NULL;
	_oldMouseX = 0;
	_oldMouseY = 0;
	_mouseX = 0;
	_mouseY = 0;
	msg.message = WM_NULL;
	_hWnd = 0;
	Initialise(hInstance);
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	InitD3D(_hWnd);
	_frame = (this);
	_controller = new GamePadController();
	// Create Scenegraph
	_scene = new SceneGraph(_pd3dDevice);
}


//-----------------------------------------------------------------------------
// Name: Initialise
// Desc: Initializes Window
//-----------------------------------------------------------------------------
bool Framework::Initialise(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = windPROC; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Create the application's window
	_hWnd = CreateWindow(L"D3DWndClassName", L"Framework", WS_OVERLAPPEDWINDOW, 100, 100, 1024 ,800, 0, 0, wc.hInstance, 0);

	if(!_hWnd)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	UnregisterClass( L"Framework", wc.hInstance );
	return 0;
}

//-----------------------------------------------------------------------------
// Name: Run()
// Desc: Process Windows Messages
//-----------------------------------------------------------------------------
int Framework::Run(void)
{
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER nextTime;				// Time to render next frame
	LARGE_INTEGER currentTime;
	LARGE_INTEGER lastTime;
	double timeFactor;
	double _timeSpan;
	DWORD msPerFrame = 40;				// MS per frame - default if no performance counter is available
	bool usePerformanceCounter = false;
	bool updateFlag = true;
	if (QueryPerformanceFrequency(&counterFrequency))
	{
		// We can use a performance counter
		usePerformanceCounter = true;
		msPerFrame = (DWORD)(counterFrequency.QuadPart / 25);
		timeFactor = 1.0 / counterFrequency.QuadPart;
		QueryPerformanceCounter(&nextTime);
		lastTime = nextTime;
	}
	else
	{
		// We do not have a performance counter, so use timeGetTime
		nextTime.QuadPart = timeGetTime();
		timeFactor = 0.001;
		lastTime = nextTime;
	}

	msg.message = WM_NULL;
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			if (updateFlag)
			{
				if (usePerformanceCounter)
				{
					QueryPerformanceCounter(&currentTime);
				}
				else
				{
					currentTime.QuadPart = timeGetTime();
				}
				_timeSpan = (currentTime.QuadPart - lastTime.QuadPart) * timeFactor;
				lastTime = currentTime;
				// _timeSpan should be made available to all of the scene nodes so that they know
				// how much time has elapsed since the last update.
				Update();
				updateFlag = false;
			}
			// Use appropriate mechanism to get time
			if (usePerformanceCounter)
			{
				QueryPerformanceCounter(&currentTime);
			}
			else
			{
				currentTime.QuadPart = timeGetTime();
			}
			// Is it time to render the frame?
			if (currentTime.QuadPart > nextTime.QuadPart)
			{
				Render();
				// Set time for next frame
				nextTime.QuadPart += msPerFrame;
				// If we get more than a frame ahead, allow one to be dropped
				// Otherwise, we will never catch up if we let the error accumulate
				// and message handling will suffer
				if (nextTime.QuadPart < currentTime.QuadPart)
				{
					nextTime.QuadPart = currentTime.QuadPart + msPerFrame;
				}
				updateFlag = true;
			}

		}
	}
	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT Framework::InitD3D(HWND hWnd)
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if( NULL == (_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}
	// Set up the structure used to create the D3DDevice. Most parameters are
	// zeroed out. We set Windowed to TRUE, since we want to do D3D in a
	// window, and then set the SwapEffect to "discard", which is the most
	// efficient method of presenting the back buffer to the display.  And 
	// we request a back buffer format that matches the current desktop display 
	// format.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;


	// Create the Direct3D device. Here we are using the default adapter (most
	// systems only have one, unless they have multiple graphics hardware cards
	// installed) and requesting the HAL (which is saying we want the hardware
	// device rather than a software one). Software vertex processing is 
	// specified since we know it will work on all cards. On cards that support 
	// hardware vertex processing, though, we would see a big performance gain 
	// by specifying hardware vertex processing.

	if( FAILED( _pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

		// Turn on solid, gouraud shading
	_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	// Use an ambient light of white
	_pd3dDevice->SetRenderState( D3DRS_AMBIENT, D3DXCOLOR(255, 255, 255, 255));
	//// and turn on lighting
	_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	// Set the Sampler State to blend terrain & reduce "Blockiness"
	_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );


	if(!CheckDeviceCapabilities())
	{
		MessageBox(0, L"Device Capabilities Check Failed", 0, 0);
		PostQuitMessage(0);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Framework::Shutdown()
{
	if(_pd3dDevice)
	{
		SAFE_RELEASE(_pd3dDevice);
	}

	if(_pD3D)
	{
		SAFE_RELEASE(_pD3D);
	}
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Framework::Render()
{
	_scene->Render();
}

//-----------------------------------------------------------------------------
// Name: windPROC()
// Desc: The window's procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK Framework::windPROC(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (_frame != NULL)
	{
		return _frame->MsgProc(hWnd, msg,  wParam,  lParam );
	}
	else
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT Framework::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	float _amountOfMovement = 0.5f;
	float _rotationAmount = 0.015f;
	switch( msg )
	{
	case WM_DESTROY:
		_frame->Shutdown();
		PostQuitMessage( 0 );
		return 0;

	case WM_PAINT:
		_frame->Update();
		_frame->Render();
		ValidateRect(hWnd, NULL);
		return 0;

	case WM_MOUSEMOVE:
	{
		// Store the old mouse positions
		_oldMouseX = _mouseX;
		_oldMouseY = _mouseY;

		// Retrieve mouse screen position
		_mouseX = (short)LOWORD(lParam);
		_mouseY = (short)HIWORD(lParam);

		if (_mouseY < 200)
		{
			_mouseY = 200;
		}
		
		if (_mouseY > 450)
		{
			_mouseY = 450;
		}

		if ((_mouseX -_oldMouseX) > 0)
		{
			_renderCamera->Yaw(_rotationAmount * 1);
		}

		if ((_mouseX -_oldMouseX) < 0)
		{
			_renderCamera->Yaw(_rotationAmount * -1);
		}
	}

	case WM_KEYDOWN:
		switch (wParam)
		{
			// To move forward
		case 'q':
		case 'Q':
			_renderCamera->MoveForward(_amountOfMovement);
			break;

			// To move backward
		case 'w':
		case 'W':
			_renderCamera->MoveForward(_amountOfMovement * -1);
			break;

			//To move camera to the left
		case VK_LEFT:
			_renderCamera->MoveRight(_amountOfMovement * -1);
			break;

			//To move camera to the right
		case VK_RIGHT:
			_renderCamera->MoveRight(_amountOfMovement);
			break;

			//To move camera up
		case 'a':
		case 'A':
			_renderCamera->MoveUp(_amountOfMovement);
			break;

			//To move camera down
		case 's':
		case 'S':
			_renderCamera->MoveUp(_amountOfMovement * -1);
			break;

			//Change to 1st person camera
		case '1':
			_tank->SetCameraType(false);
			_renderCamera->SetCameraType(false);
			break;

			//Change to 3rd person camera
		case '2':
			_tank->SetCameraType(true);
			_renderCamera->SetCameraType(true);
			break;

		default:
			break;
		}
	default:
	return DefWindowProc(hWnd, msg, wParam, lParam );
	}
}

//-----------------------------------------------------------------------------
// Name: CheckDeviceCapabilities()
// Desc: Checks Device Capabilities
//-----------------------------------------------------------------------------
bool Framework::CheckDeviceCapabilities(void)
{
	 D3DCAPS9 _capability;
	_pd3dDevice->GetDeviceCaps(&_capability);

		// Check for hardware vertex processing
	if(_capability.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		return true;
	}

	// Check for Pure Device
	if(_capability.DevCaps && D3DDEVCAPS_PUREDEVICE)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name: OnLostDevice()
// Desc: On Lost Device
//-----------------------------------------------------------------------------
void Framework::OnLostDevice(void)
{
	// Get the state of the graphics device.
	HRESULT hr = _pd3dDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if( hr == D3DERR_DEVICELOST )
	{
		Sleep(20);
	}
	// Driver error, exit.
	else if( hr == D3DERR_DRIVERINTERNALERROR )
	{
		MessageBox(0, L"Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
	}
	// The device is lost but we can reset and restore it.
	else if( hr == D3DERR_DEVICENOTRESET )
	{
		OnLostDevice();
		OnResetDevice();
	}
}

//-----------------------------------------------------------------------------
// Name: OnResetDevice()
// Desc: On Reset Device
//-----------------------------------------------------------------------------
void Framework::OnResetDevice(void)
{
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Update the Framework
//-----------------------------------------------------------------------------
void Framework::Update(void)
{
	_controller->ProcessGameController();
	_scene->Update();
}

//-----------------------------------------------------------------------------
// Name: TextDisplay()
// Desc: Output Text to the screen
//-----------------------------------------------------------------------------
void Tank::TextDisplay()
{
	std::ostringstream output;
	output
		<< "1 : 1st Person Camera" << std::endl
		<< "2 : 3rd Person Camera" << std::endl
		<< "Q : Move Forward" << std::endl
		<< "W : Move Backward" << std::endl
		<< "A : Look Up" << std::endl
		<< "S : Look Down" << std::endl
		<< "Mouse : Rotate Left & Right" << std::endl;

	string _string = output.str().c_str();
	wstring _wideString;

	_wideString.assign(_string.begin(), _string.end());

	LPCWSTR _textString = _wideString.c_str();

	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, _textString, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(0,0,0));
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Set up Matrices
//-----------------------------------------------------------------------------
void Framework::SetupMatrices(void)
{
}

//Accessors & Mutators
LPDIRECT3DDEVICE9 Framework::GetDirect3dDevice(void)
{
	return this->_pd3dDevice;
}

SceneGraph * Framework:: GetSceneGraph(void)
{
	return _scene;
}

HWND Framework::GetHandle(void)
{
	return _hWnd;
}

void Framework::SetCamera(CameraRender* renderCam)
{
	_renderCamera = renderCam;

	// Make the camera accessible to the controller
	_controller->SetCamera(_renderCamera);
}


void Framework::SetTank(Tank* tank)
{
	_tank = tank;
}


CameraRender* Framework::GetCamera(void)const
{
	return _renderCamera;
}


void Framework::SetTerrain(TerrainNode* terrainNode)
{
	_terrain = terrainNode;
}

TerrainNode* Framework::GetTerrain(void)const
{
	return _terrain;
}

void Framework::GetController(void)const
{
	_controller->SetTank(_tank);
}

void Framework::SetObjects(CameraRender* camRender, Tank* tank, SkyDome* skyDome, FrameWorkResourceManager* frameResourcesManager)
{
	_renderCamera = camRender;
	_skyDome = skyDome;
	_tank = tank;
	_frameWorkResourceManager = frameResourcesManager;
}