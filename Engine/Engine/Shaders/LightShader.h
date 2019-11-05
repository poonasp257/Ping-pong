#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

class Light;

class LightShader {
private:
	struct MatrixBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBufferType {
		D3DXVECTOR3 cameraPosition;
		float padding;
	};

	struct LightBufferType {
		D3DXVECTOR3 lightDirection;
		Color ambientColor;
		Color diffuseColor;
		Color specularColor;
		float specularPower;
	};
	
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* lightBuffer;

private:
	bool initializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void outputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, const Light*, D3DXVECTOR3);
	void renderShader(ID3D11DeviceContext*, int);

public:
	LightShader();
	~LightShader();

	bool initialize(ID3D11Device*, HWND);
	bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, const Light*, D3DXVECTOR3);
	bool setFilter(ID3D11Device *device, D3D11_FILTER filterMode);
};
#endif