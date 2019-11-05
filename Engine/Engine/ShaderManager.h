#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

class ShaderManager {
private:
	std::unique_ptr<TextureShader> textureShader;
	std::unique_ptr<FontShader> fontShader;
	std::unique_ptr<LightShaderClass> lightShader;

public:
	ShaderManager();
	~ShaderManager();

	bool initialize(ID3D11Device*, HWND, D3DXMATRIX);

	bool renderTextureShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	bool renderFontShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
	bool renderLightShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4,
		D3DXVECTOR3, D3DXVECTOR4, float);
};
#endif