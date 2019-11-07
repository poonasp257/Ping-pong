#include "stdafx.h"

Model::Model() : texture(new Texture), model() {
	vertexBuffer = 0;
	indexBuffer = 0;
}

Model::~Model() {
	texture.reset();
	
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = 0;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}

bool Model::initialize(ID3D11Device* device, const WCHAR* textureFilename) {
	bool result;

	result = createPrimitive(device);
	if (!result) return false;

	result = loadTexture(device, textureFilename);
	if(!result) return false;

	return true;
}

bool Model::initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename) {
	bool result;

	result = loadModel(modelFilename);
	if (!result) return false;

	result = initializeBuffers(device);
	if (!result) return false;

	result = loadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}

void Model::render(ID3D11DeviceContext* deviceContext) {
	renderBuffers(deviceContext);
}

bool Model::createPrimitive(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexCount = 4;
	indexCount = 6;
	
	vertices = new VertexType[vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[indexCount];
	if (!indices) return false;

	model = std::make_unique<ModelType[]>(vertexCount);

	vertices[0].position = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[1].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);
	vertices[1].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[2].position = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[3].position = D3DXVECTOR3(10.0f, -10.0f, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	model[0].position = vertices[0].position;
	model[0].texture = vertices[0].texture;
	model[1].position = vertices[1].position;
	model[1].texture = vertices[1].texture;
	model[2].position = vertices[2].position;
	model[2].texture = vertices[2].texture;
	model[3].position = vertices[3].position;
	model[3].texture = vertices[3].texture;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	polygonCount = 2;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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

bool Model::initializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[indexCount];
	if (!indices) return false;

	for (int i = 0; i < vertexCount; ++i) {		
		vertices[i].position = model[i].position;
		vertices[i].texture = model[i].texture;
		vertices[i].normal = model[i].normal;

		indices[i] = i;
	}
	   
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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

void Model::renderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType); 
	offset = 0;
    
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::loadTexture(ID3D11Device* device, const WCHAR* filename) {
	bool result = texture->initialize(device, filename);
	if(!result) return false;

	return true;
}

bool Model::readFileCounts(const WCHAR* fileName, int& textureCount, int& normalCount, int& faceCount) {
	std::ifstream fin;
	char input;

	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	fin.open(fileName);
	if (fin.fail() == true) return false;

	fin.get(input);
	while (!fin.eof()) {
		if (input == 'v') {
			fin.get(input);
			if (input == 't') { ++textureCount; }
			if (input == 'n') { ++normalCount; }
		}

		if (input == 'f') {
			fin.get(input);
			if (input == ' ') { ++faceCount; }
		}

		while (input != '\n') {
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;
}

bool Model::loadModel(const WCHAR* fileName) {
	std::ifstream fin;
	int textureCount, normalCount, faceCount;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex, mIndex;
	char input, input2;

	bool result = readFileCounts(fileName, textureCount, normalCount, faceCount);
	if (!result) return false;

	indexCount = vertexCount = faceCount * 3;
	polygonCount = faceCount;

	std::unique_ptr<D3DXVECTOR3[]> vertices(new D3DXVECTOR3[vertexCount]);
	std::unique_ptr<D3DXVECTOR2[]> texcoords(new D3DXVECTOR2[textureCount]);
	std::unique_ptr<D3DXVECTOR3[]> normals(new D3DXVECTOR3[normalCount]);
	std::unique_ptr<FaceType[]> faces(new FaceType[faceCount]);

	model = std::make_unique<ModelType[]>(vertexCount);
	   
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;
	
	fin.open(fileName);
	
	if (fin.fail()) return false;

	fin.get(input);
	while (!fin.eof()) {
		if (input == 'v') {
			fin.get(input);

			if (input == ' ') {
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				++vertexIndex;
			}
			else if (input == 't') {
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				++texcoordIndex;
			}
			else if (input == 'n') {
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				++normalIndex;
			}
		}
		else if (input == 'f') {
			fin.get(input);
			if (input == ' ') {
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2
					>> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				++faceIndex;
			}
		}

		while (input != '\n') {
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	for (int i = 0; i < faceIndex; ++i) {
		mIndex = i * 3;

		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		model[mIndex].position = vertices[vIndex];
		model[mIndex].texture = texcoords[tIndex];
		model[mIndex].normal = normals[nIndex];
		
		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		model[mIndex + 1].position = vertices[vIndex];
		model[mIndex + 1].texture = texcoords[tIndex];
		model[mIndex + 1].normal = normals[nIndex];

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		model[mIndex + 2].position = vertices[vIndex];
		model[mIndex + 2].texture = texcoords[tIndex];
		model[mIndex + 2].normal = normals[nIndex];
	}

	return true;
}