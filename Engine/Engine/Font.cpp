#include "stdafx.h"

Font::Font() : font(new FontType[95]),
	texture(std::make_shared<Texture>()) {

}

Font::~Font() {
	texture.reset();
}

bool Font::initialize(ID3D11Device* device, const char* fontFilename, const WCHAR* textureFilename) {
	bool result;
	
	result = loadFontData(fontFilename);
	if(!result) return false;

	result = loadTexture(device, textureFilename);
	if(!result) return false;

	return true;
}

bool Font::loadFontData(const char* filename) {
	std::ifstream fin;
	char temp;

	fin.open(filename);
	if(fin.fail()) return false;
	
	for(int i = 0; i < 95; ++i) {
		fin.get(temp);
		while(temp != ' ') fin.get(temp);

		fin.get(temp);
		while(temp != ' ') fin.get(temp);

		fin >> font.get()[i].left;
		fin >> font.get()[i].right;
		fin >> font.get()[i].size;
	}
	
	fin.close();

	return true;
}

bool Font::loadTexture(ID3D11Device* device, const WCHAR* filename) {
	bool result = texture->initialize(device, filename);
	if(!result) return false;

	return true;
}

ID3D11ShaderResourceView* Font::getTexture() {
	return texture->getTexture();
}

void Font::buildVertexArray(void* vertices, const char* sentence, float drawX, float drawY) {
	VertexType* vertexPtr;
	int numLetters, index, letter;

	vertexPtr = (VertexType*)vertices;
	numLetters = (int)strlen(sentence);
	index = 0;

	// Draw each letter onto a quad.
	for(int i=  0; i < numLetters; ++i) {
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0) {
			drawX = drawX + 3.0f;
		}
		else {
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(font.get()[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + font.get()[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(font.get()[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(font.get()[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(font.get()[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + font.get()[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(font.get()[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + font.get()[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(font.get()[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + font.get()[letter].size + 1.0f;
		}
	}
}