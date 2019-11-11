#ifndef POINTLIGHTSHADER_H
#define POINTLIGHTSHADER_H

const int NUM_LIGHTS = 1;

class PointLightShader {
private:
	struct MatrixBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightColorBufferType {
		Color diffuseColor[NUM_LIGHTS];
	};

	struct LightPositionBufferType {
		D3DXVECTOR4 lightPosition[NUM_LIGHTS];
	};

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;

private:
	bool initializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void outputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, const Light*);
	void renderShader(ID3D11DeviceContext*, int);

public:
	PointLightShader();
	~PointLightShader();

	bool initialize(ID3D11Device*, HWND);
	bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, const Light*);
};
#endif