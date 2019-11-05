#ifndef TRANSFORM_H
#define TRANSFORM_H

class Transform : public Component {
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

public:
	Transform(GameObject*, const D3DXVECTOR3&, const D3DXVECTOR3&, const D3DXVECTOR3&);
	~Transform();

	void start() {};
	void update() {};

	D3DXVECTOR3 getPosition() const { return position; }
	D3DXVECTOR3 getRotation() const { return rotation; }
	D3DXVECTOR3 getScale() const { return scale; }

	void setPosition(const D3DXVECTOR3& pos) { this->position = pos; }
	void setRotation(const D3DXVECTOR3& rotation) { this->rotation = rotation; }
	void setScale(const D3DXVECTOR3& scale) { this->scale = scale; }
};
#endif