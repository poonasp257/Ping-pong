#include "stdafx.h"

Direct3D::Direct3D() {
	swapChain = 0;
	device = 0;
	deviceContext = 0;
	renderTargetView = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;
	depthDisabledStencilState = 0;
	alphaEnableBlendingState = 0;
	alphaDisableBlendingState = 0;
}

Direct3D::Direct3D(const Direct3D& other) {

}

Direct3D::~Direct3D() {
	if (swapChain) {
		swapChain->SetFullscreenState(false, NULL);
	}

	if (rasterState) {
		rasterState->Release();
		rasterState = 0;
	}

	if (depthStencilView) {
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if (depthStencilState) {
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if (depthStencilBuffer) {
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if (renderTargetView) {
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if (deviceContext) {
		deviceContext->Release();
		deviceContext = 0;
	}

	if (device) {
		device->Release();
		device = 0;
	}

	if (swapChain) {
		swapChain->Release();
		swapChain = 0;
	}

	if (depthDisabledStencilState) {
		depthDisabledStencilState->Release();
		depthDisabledStencilState = 0;
	}

	if (alphaEnableBlendingState) {
		alphaEnableBlendingState->Release();
		alphaEnableBlendingState = 0;
	}

	if (alphaDisableBlendingState) {
		alphaDisableBlendingState->Release();
		alphaDisableBlendingState = 0;
	}
}

bool Direct3D::initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, 
						  float screenDepth, float screenNear) {
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator = 0, denominator = 0, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;

	vsync_enabled = vsync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result)) return false;

	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result)) return false;

	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result)) return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result)) return false;

	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList) return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result)) return false;

	for(i=0; i<numModes; ++i) {
		if(displayModeList[i].Width == (unsigned int)screenWidth) {
			if(displayModeList[i].Height == (unsigned int)screenHeight) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result)) return false;

	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0) return false;

	delete [] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if(vsync_enabled) {
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	if(fullscreen) {
		swapChainDesc.Windowed = false;
	}
	else {
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
										   D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	if(FAILED(result)) return false;

	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result)) return false;

	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if(FAILED(result)) return false;

	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if(FAILED(result)) return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if(FAILED(result)) return false;

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if(FAILED(result)) return false;

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if(FAILED(result)) return false;

	deviceContext->RSSetState(rasterState);
	
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    deviceContext->RSSetViewports(1, &viewport);

	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
    D3DXMatrixIdentity(&worldMatrix);

	D3DXMatrixOrthoLH(&orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState);
	if (FAILED(result)) return false;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = device->CreateBlendState(&blendStateDescription, &alphaEnableBlendingState);
	if (FAILED(result)) return false;
	
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	result = device->CreateBlendState(&blendStateDescription, &alphaDisableBlendingState);
	if (FAILED(result)) return false;

    return true;
}

void Direct3D::beginScene(float red, float green, float blue, float alpha) {
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::endScene() {
	if(vsync_enabled) {
		swapChain->Present(1, 0);
	}
	else {
		swapChain->Present(0, 0);
	}
}

ID3D11Device* Direct3D::getDevice() {
	return device;
}

ID3D11DeviceContext* Direct3D::getDeviceContext() {
	return deviceContext;
}

void Direct3D::getProjectionMatrix(D3DXMATRIX& projectionMatrix) const {
	projectionMatrix = this->projectionMatrix;
}

void Direct3D::getWorldMatrix(D3DXMATRIX& worldMatrix) const {
	worldMatrix = this->worldMatrix;
}

void Direct3D::getOrthoMatrix(D3DXMATRIX& orthoMatrix) const {
	orthoMatrix = this->orthoMatrix;
}

void Direct3D::getVideoCardInfo(char* cardName, int& memory) const {
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}

void Direct3D::turnZBufferOn() {
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}

void Direct3D::turnZBufferOff() {
	deviceContext->OMSetDepthStencilState(depthDisabledStencilState, 1);
}

void Direct3D::turnOnAlphaBlending() {
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	deviceContext->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);
}void Direct3D::turnOffAlphaBlending() {
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	deviceContext->OMSetBlendState(alphaDisableBlendingState, blendFactor, 0xffffffff);
}