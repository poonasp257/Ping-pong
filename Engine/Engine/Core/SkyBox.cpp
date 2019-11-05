#include "stdafx.h"

SkyBox::SkyBox() {

}

SkyBox::~SkyBox() {
	sphereIndexBuffer->Release();
	sphereVertBuffer->Release();

	SKYMAP_VS->Release();
	SKYMAP_PS->Release();
	SKYMAP_VS_Buffer->Release();
	SKYMAP_PS_Buffer->Release();

	smrv->Release();
}

bool SkyBox::initialize(ID3D11Device* device, HWND hwnd, const WCHAR* textureFileName) {
	bool result;

	result = createSphere(device, 10, 10);
	if (!result) {
		return false;
	}

	result = initializeShader(device, hwnd);
	if (!result) {
		return false;
	}

	result = loadTexture(device, textureFileName);
	if (!result) {
		return false;
	}

	return true;
}

bool SkyBox::createSphere(ID3D11Device* device, int latLines, int longLines) {
	numSphereVertices = ((latLines - 2) * longLines) + 2;
	numSphereFaces = ((latLines - 3)*(longLines) * 2) + (longLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(numSphereVertices);

	D3DXVECTOR3 currVertPos = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	D3DXMATRIX rotationX, rotationY;
	D3DXVECTOR3 up = { 0.0f, 0.0f, 1.0f };

	for (DWORD i = 0; i < latLines - 2; ++i) {
		spherePitch = (i + 1) * (3.14 / (latLines - 1));
		D3DXMatrixRotationX(&rotationX, spherePitch);
		
		for (DWORD j = 0; j < longLines; ++j) {
			sphereYaw = j * (6.28 / (longLines));
			D3DXMATRIX rotationMat = rotationX * rotationY;

			D3DXMatrixRotationZ(&rotationY, sphereYaw);
			D3DXVec3TransformNormal(&currVertPos, &up, &rotationMat);

			D3DXVec3Normalize(&currVertPos, &currVertPos);
			vertices[i*longLines + j + 1].pos.x = currVertPos.x;
			vertices[i*longLines + j + 1].pos.y = currVertPos.y;
			vertices[i*longLines + j + 1].pos.z = currVertPos.z;
		}
	}

	vertices[numSphereVertices - 1].pos.x = 0.0f;
	vertices[numSphereVertices - 1].pos.y = 0.0f;
	vertices[numSphereVertices - 1].pos.z = -1.0f;
	
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * numSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);
	
	std::vector<DWORD> indices(numSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < longLines - 1; ++l) {
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = longLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < latLines - 3; ++i) {
		for (DWORD j = 0; j < longLines - 1; ++j) {
			indices[k] = i * longLines + j + 1;
			indices[k + 1] = i * longLines + j + 2;
			indices[k + 2] = (i + 1)*longLines + j + 1;

			indices[k + 3] = (i + 1)*longLines + j + 1;
			indices[k + 4] = i * longLines + j + 2;
			indices[k + 5] = (i + 1)*longLines + j + 2;

			k += 6;
		}

		indices[k] = (i*longLines) + longLines;
		indices[k + 1] = (i*longLines) + 1;
		indices[k + 2] = ((i + 1)*longLines) + longLines;

		indices[k + 3] = ((i + 1)*longLines) + longLines;
		indices[k + 4] = (i*longLines) + 1;
		indices[k + 5] = ((i + 1)*longLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < longLines - 1; ++l) {
		indices[k] = numSphereVertices - 1;
		indices[k + 1] = (numSphereVertices - 1) - (l + 1);
		indices[k + 2] = (numSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = numSphereVertices - 1;
	indices[k + 1] = (numSphereVertices - 1) - longLines;
	indices[k + 2] = numSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * numSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(SkyBoxBufferType);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	device->CreateBuffer(&cbbd, NULL, &skyboxBuffer);

	return true;
}

bool SkyBox::initializeShader(ID3D11Device* device, HWND hwnd) {
	D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "D2D_PS", "ps_4_0", 0, 0, 0, &D2D_PS_Buffer, 0, 0);

	D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	D3DX11CompileFromFile(L"../Engine/Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);
	
	device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	
	device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
	
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FrontCounterClockwise = false;
	device->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	return true;
}

bool SkyBox::loadTexture(ID3D11Device* device, const WCHAR* fileName) {
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* SMTexture = 0;
	D3DX11CreateTextureFromFile(device, fileName,
		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
		
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);

	return true;
}
 
bool SkyBox::render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	bool result;

	result = setShaderParameters(deviceContext, worldMatrix,
		viewMatrix, projectionMatrix);
	if (!result) return false;

	renderShader(deviceContext);

	return true;
}

bool SkyBox::setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the spheres vertex buffer
	deviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	// Set the world view projection matrix and send it to the constant buffer in effect file
	D3DXMATRIX WVP = worldMatrix * viewMatrix * projectionMatrix;
	SkyBoxBufferType skyBox;

	D3DXMatrixTranspose(&skyBox.WVP, &WVP);
	D3DXMatrixTranspose(&skyBox.world, &worldMatrix);

	deviceContext->UpdateSubresource(skyboxBuffer, 0, NULL, &skyBox, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &skyboxBuffer);
	// Send our skymap resource view to pixel shader
	deviceContext->PSSetShaderResources(0, 1, &smrv);

	return true;
}

void SkyBox::renderShader(ID3D11DeviceContext* deviceContext) {
	// Set the new VS and PS shaders
	deviceContext->VSSetShader(SKYMAP_VS, 0, 0);
	deviceContext->PSSetShader(SKYMAP_PS, 0, 0);
	// Set the new depth/stencil and RS states

	deviceContext->OMSetDepthStencilState(DSLessEqual, 0);
	deviceContext->RSSetState(RSCullNone);
	deviceContext->DrawIndexed(numSphereFaces * 3, 0, 0);

	// Set the default VS shader and depth/stencil state
	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->OMSetDepthStencilState(NULL, 0);
}