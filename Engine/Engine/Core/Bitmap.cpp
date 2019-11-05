#include "stdafx.h"

Bitmap::Bitmap() : vertexBuffer(nullptr), indexBuffer(nullptr), 
	texture(std::make_shared<Texture>()){
	
}

Bitmap::~Bitmap() {
	texture.reset();

	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

bool Bitmap::initialize(ID3D11Device* device, int screenWidth, int screenHeight, const WCHAR* textureFilename, int
	bitmapWidth, int bitmapHeight) {
	bool result;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->bitmapWidth = bitmapWidth;
	this->bitmapHeight = bitmapHeight;
	previousPosX = -1;
	previousPosY = -1;

	result = initializeBuffers(device);
	if (!result) return false;

	result = loadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}

bool Bitmap::render(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {
	bool result = updateBuffers(deviceContext, positionX, positionY);
	if (!result) return false;

	renderBuffers(deviceContext);

	return true;
}

int Bitmap::getIndexCount() const {
	return indexCount;
}

ID3D11ShaderResourceView* Bitmap::getTexture() {
	return texture->getTexture();
}

bool Bitmap::initializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexCount = 6;
	indexCount = vertexCount;

	vertices = new VertexType[vertexCount];
	if (!vertices) return false;
	
	indices = new unsigned long[indexCount];
	if (!indices) return false;

	memset(vertices, 0, (sizeof(VertexType) * vertexCount));

	for (int i = 0; i < indexCount; i++) {
		indices[i] = i;
	}


	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) return false;

	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}

bool Bitmap::updateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	if ((positionX == previousPosX) 
		&& (positionY == previousPosY)) return true;

	previousPosX = positionX;
	previousPosY = positionY;
	
	left = (float)((screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)bitmapWidth;
	top = (float)(screenHeight / 2) - (float)positionY;
	bottom = top - (float)bitmapHeight;

	vertices = new VertexType[vertexCount];
	if (!vertices) return false;

	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * vertexCount));
	deviceContext->Unmap(vertexBuffer, 0);
	
	delete[] vertices;
	vertices = 0;
	
	return true;
}

void Bitmap::renderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Bitmap::loadTexture(ID3D11Device* device, const WCHAR* filename) {
	bool result = texture->initialize(device, filename);
	if (!result) return false;

	return true;
}