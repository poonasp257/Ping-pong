#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

class TextureShader {
private:
	struct MatrixBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

private:
	ID3D11VertexShader	*vertexShader;
	ID3D11PixelShader	*pixelShader;
	ID3D11InputLayout	*layout;
	ID3D11Buffer		*matrixBuffer;
	ID3D11SamplerState	*sampleState;

private:
	bool initializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void outputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void renderShader(ID3D11DeviceContext*, int);

public:
	TextureShader();
	~TextureShader();

	bool initialize(ID3D11Device*, HWND);
	bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	bool setFilter(ID3D11Device *device, D3D11_FILTER filterMode);
};
#endif