#ifndef FONTSHADER_H
#define FONTSHADER_H

class FontShader {
private:
	struct ConstantBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct PixelBufferType {
		D3DXVECTOR4 pixelColor;
	};

private:
	ID3D11VertexShader	*vertexShader;
	ID3D11PixelShader	*pixelShader;
	ID3D11InputLayout	*layout;
	ID3D11Buffer		*constantBuffer;
	ID3D11SamplerState	*sampleState;
	ID3D11Buffer		*pixelBuffer;

private:
	bool initializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void outputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
	void renderShader(ID3D11DeviceContext*, int);

public:
	FontShader();
	~FontShader();

	bool initialize(ID3D11Device*, HWND);
	bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
};
#endif