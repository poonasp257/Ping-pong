#include "stdafx.h"

Font::Font() {
	m_Font = 0;
	m_Texture = 0;
}

Font::~Font() {
	// Release the font data array.
	if (m_Font) {
		delete[] m_Font;
		m_Font = 0;
	}

	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

bool Font::initialize(ID3D11Device* device, const char* fontFilename, const WCHAR* textureFilename) {
	bool result;
	
	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if(!result) return false;

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if(!result) return false;

	return true;
}

bool Font::loadFontData(const char* filename) {
	std::ifstream fin;
	int i;
	char temp;

	m_Font = new FontType[95];
	if(!m_Font) return false;

	fin.open(filename);
	if(fin.fail()) return false;

	for(int i = 0; i < 95; ++i) {
		fin.get(temp);
		while(temp != ' ') fin.get(temp);

		fin.get(temp);
		while(temp != ' ') fin.get(temp);

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	fin.close();

	return true;
}

bool Font::loadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* Font::getTexture() const {
	return m_Texture->GetTexture();
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
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}