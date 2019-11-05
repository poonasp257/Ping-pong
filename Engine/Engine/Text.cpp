#include "stdafx.h"

Text::Text(const ShaderManager* shaderManager) : font(new Font),
	shaderManager(shaderManager), isEnableRenderInfo(false) {

}

Text::~Text() {
	for (auto info : renderInfo) {
		releaseSentence(&info);
	}
}

bool Text::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, 
						   D3DXMATRIX viewMatrix) {
	bool result;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	baseViewMatrix = viewMatrix;

	result = font->initialize(device, "../Engine/data/fontdata.txt", L"../Engine/data/font.dds");
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	renderInfo.reserve(7);

	for (int i = 0; i < renderInfo.capacity(); ++i) {
		SentenceType* sentence = new SentenceType;

		result = initializeSentence(&sentence, 20, 20 * (i + 1), MAX_LENGTH, device);
		if (!result) {
			delete sentence;
			return false;
		}

		renderInfo.push_back(sentence);
	}
	
	return true;
}

void Text::render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix) {
	if (!isEnableRenderInfo) return;

	for (auto info : renderInfo) {
		renderSentence(deviceContext, info, worldMatrix, orthoMatrix);
	}
}

bool Text::initializeSentence(SentenceType** sentence, int x, int y, int maxLength, ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
		
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;
	(*sentence)->x = x;
	(*sentence)->y = y;
	(*sentence)->color = { 1.0f, 1.0f, 1.0f, 1.0f };

	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices) return false;

	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices) return false;

	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	for(int i = 0; i < (*sentence)->indexCount; ++i) {
		indices[i] = i;
	}

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result)) return false;
	
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result)) return false;

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool Text::updateSentence(ID3D11DeviceContext* deviceContext, 
	SentenceType* sentence, const char* text, Color color = { 0.0f, 1.0f, 0.0f, 1.0f }) {
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	sentence->color = color;
	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength) return false;

	vertices = new VertexType[sentence->vertexCount];
	if (!vertices) return false;

	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));
	drawX = (float)(((this->screenWidth / 2) * -1) + sentence->x);
	drawY = (float)((this->screenHeight / 2) - sentence->y);
	
	font->buildVertexArray((void*)vertices, text, drawX, drawY);

	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	deviceContext->Unmap(sentence->vertexBuffer, 0);
	delete [] vertices;
	vertices = 0;

	return true;
}

void Text::releaseSentence(SentenceType** sentence) {
	if(*sentence) {
		if((*sentence)->vertexBuffer) {
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if((*sentence)->indexBuffer) {
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete *sentence;
		*sentence = 0;
	}
}

bool Text::renderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, 
							   D3DXMATRIX orthoMatrix) {
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;
	
    stride = sizeof(VertexType); 
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = D3DXVECTOR4(sentence->color.r, sentence->color.g,
		sentence->color.b, sentence->color.a);

	result = shaderManager->getFontShader()->render(deviceContext, sentence->indexCount, worldMatrix,
		baseViewMatrix, orthoMatrix, font->getTexture(), pixelColor);
	if (!result) return false;

	return true;
}

bool Text::setCameraPosition(const D3DXVECTOR3& pos, ID3D11DeviceContext* deviceContext) {
	std::string buffer;
	buffer += "Camera Position = (";
	buffer += std::to_string((int)pos.x) + ", ";
	buffer += std::to_string((int)pos.y) + ", ";
	buffer += std::to_string((int)pos.z);
	buffer += ')';

	bool result = updateSentence(deviceContext, renderInfo[0], buffer.c_str());
	if (!result) return false;
	
	return true;
}

bool Text::setFPS(int fps, ID3D11DeviceContext* deviceContext) {
	Color color;
	std::string buffer;
	buffer += "FPS: ";
	buffer += std::to_string((int)fps);

	if (fps > 9999) {
		fps = 9999;
	}

	if (fps >= 60) {
		color.r = 0.0f;
		color.g = 1.0f;
		color.b = 0.0f;
	}
	else if (fps < 60) {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 0.0f;
	} 
	else if (fps < 30) {
		color.r = 1.0f;
		color.g = 0.0f;
		color.b = 0.0f;
	}

	bool result = updateSentence(deviceContext, renderInfo[1], buffer.c_str(), color);
	if (!result) return false;

	return true;
}

bool Text::setCPU(int cpu, ID3D11DeviceContext* deviceContext) {
	std::string buffer;
	buffer += "Cpu: ";
	buffer += std::to_string((int)cpu);
	buffer += "%";

	bool result = updateSentence(deviceContext, renderInfo[2], buffer.c_str());
	if (!result) return false;

	return true;
}

bool Text::setNumOfObjects(int num, ID3D11DeviceContext* deviceContext) {
	std::string buffer;
	buffer += "Objects: ";
	buffer += std::to_string((int)num);

	bool result = updateSentence(deviceContext, renderInfo[3], buffer.c_str());
	if (!result) return false;

	return true;
}

bool Text::setNumOfPolygons(int num, ID3D11DeviceContext* deviceContext) {
	std::string buffer;
	buffer += "Polygons: ";
	buffer += std::to_string((int)num);

	bool result = updateSentence(deviceContext, renderInfo[4], buffer.c_str());
	if (!result) return false;

	return true;
}

void Text::turnOnOffRenderInfo() {
	isEnableRenderInfo = !isEnableRenderInfo;
}