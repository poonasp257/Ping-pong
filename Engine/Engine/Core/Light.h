#ifndef LIGHT_H
#define LIGHT_H

class Light {
private:
	D3DXVECTOR4 position;
	D3DXVECTOR3 direction;
	Color ambientColor;
	Color diffuseColor;
	Color specularColor;
	float specularPower;

public:
	Light();
	~Light();

	void setDirection(float, float, float);
	void setPosition(float, float, float);
	void setDiffuseColor(float, float, float, float);
	void setAmbientColor(float, float, float, float);
	void setSpecularColor(float, float, float, float);
	void setSpecularPower(float);
	
	D3DXVECTOR3 getDirection() const { return direction; }
	D3DXVECTOR4 getPosition() const { return position; }
	Color getAmbientColor() const { return ambientColor; }
	Color getDiffuseColor() const { return diffuseColor; }
	Color getSpecularColor() const { return specularColor; }

	float getSpecularPower() const { return specularPower; }
};
#endif