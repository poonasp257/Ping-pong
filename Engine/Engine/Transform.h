#pragma once
class Transform : public Component {
private:
	D3DXMATRIX position;
	D3DXMATRIX rotattion;
	D3DXMATRIX scale;

public:
	Transform();
	~Transform();

	//void setPosition();

};

