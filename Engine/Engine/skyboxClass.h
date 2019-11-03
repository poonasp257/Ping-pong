#ifndef SKYBOXCLASS_H
#define SKYBOXCLASS_H

#include <d3d11.h>
#include <d3dx11async.h>
#include <vector>

#include "d3dclass.h"

class SkyBoxClass
{
public:
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		D3DXVECTOR3 pos;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
	};

	struct SkyBoxBufferType {
		D3DXMATRIX WVP;
		D3DXMATRIX world;
	};

public:
	SkyBoxClass();
	~SkyBoxClass();

	bool Initialize(ID3D11Device*, HWND, const WCHAR* textureFileName);
	bool InitializeShader(ID3D11Device*, HWND);
	bool CreateSphere(ID3D11Device* device, int latLines, int longLines);
	bool LoadTexture(ID3D11Device* device, const WCHAR* fileName);
	
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*);

private:
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;
	ID3D11Buffer* skyboxBuffer;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11PixelShader* D2D_PS;
	ID3D10Blob* D2D_PS_Buffer;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int numSphereVertices;
	int numSphereFaces;
};

#endif