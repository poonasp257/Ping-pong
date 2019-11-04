#ifndef DIRECT3D_H_
#define DIRECT3D_H_

class Direct3D {
private:
	bool vsync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX orthoMatrix;
	ID3D11DepthStencilState* depthDisabledStencilState;
	ID3D11BlendState* alphaEnableBlendingState;
	ID3D11BlendState* alphaDisableBlendingState;

public:
	Direct3D();
	Direct3D(const Direct3D&);
	~Direct3D();

	bool initialize(int, int, bool, HWND, bool, float, float);

	void beginScene(float, float, float, float);
	void endScene();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void getProjectionMatrix(D3DXMATRIX&) const;
	void getWorldMatrix(D3DXMATRIX&) const;
	void getOrthoMatrix(D3DXMATRIX&) const;

	void getVideoCardInfo(char*, int&) const;

	void turnZBufferOn();
	void turnZBufferOff();

	void turnOnAlphaBlending();
	void turnOffAlphaBlending();
};
#endif