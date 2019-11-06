#ifndef TRANSFORM_H
#define TRANSFORM_H

class Transform : public Component {
private:
	D3DXMATRIX worldMatrix;
	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scaleMatrix;

	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

public:
	Transform(GameObject* gameObject, const D3DXVECTOR3& position = { 0.0f, 0.0f, 0.0f },
		const D3DXVECTOR3& rotation = { 0.0f, 0.0f, 0.0f },
		const D3DXVECTOR3& scale = { 1.0f, 1.0f, 1.0f });
	~Transform();

	void start();
	void update();

	D3DXMATRIX getWorldMatrix() const { return worldMatrix; }
	D3DXVECTOR3 getPosition() const { return position; }
	D3DXVECTOR3 getRotation() const { return rotation; }
	D3DXVECTOR3 getScale() const { return scale; }

	void setPosition(const D3DXVECTOR3& pos) { this->position = pos; }
	void setRotation(const D3DXVECTOR3& rotation) { this->rotation = rotation; }
	void setScale(const D3DXVECTOR3& scale) { this->scale = scale; }
};
#endif