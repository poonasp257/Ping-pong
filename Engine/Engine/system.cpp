#include "stdafx.h"
#include "MainScene.h"

System::System() : fpsManager(std::make_unique<FPSManager>()),
	cpuManager(std::make_unique<CPUManager>()),
	timer(std::make_unique<Timer>()), 
	scene(std::make_unique<MainScene>()) {
		
}

System::System(const System& other) {

}

System::~System() {
	shutdownWindows();
}

bool System::initialize() {
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	initializeWindows(screenWidth, screenHeight);

	Input.Initiailize(hinstance, hwnd);

	result = scene->initialize(screenWidth, screenHeight, hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the Scene object.", L"Error", MB_OK);
		return false;
	}

	result = timer->initialize();
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void System::run() {
	MSG msg;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));
	
	while(true) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) break;
		else {
			result = frame();
			if (!result) break;
		}
	}
}

bool System::frame() {
	Input.DetectInput(hwnd);
	
	if (Input.GetKeyDown(DIK_ESCAPE)) return false;

	timer->frame();
	fpsManager->frame();
	cpuManager->frame();

	bool result = scene->frame(fpsManager->getFPS(), timer->getTime(),
		cpuManager->getCPUPercentage());
	if(!result) return false;

	return true;
}

void System::initializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	
	ApplicationHandle = this;
	hinstance = GetModuleHandle(NULL);
	applicationName = L"Engine";

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	RegisterClassEx(&wc);

	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN) {
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = 0;
		posY = 0;
	}
	else {
		screenWidth  = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance, NULL);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	ShowCursor(false);
}


void System::shutdownWindows() {
	ShowCursor(true);

	if(FULL_SCREEN) ChangeDisplaySettings(NULL, 0);

	DestroyWindow(hwnd);
	hwnd = NULL;

	UnregisterClass(applicationName, hinstance);
	hinstance = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch(umessage) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE: {
			PostQuitMessage(0);		
			return 0;
		}
		default: {
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}