#ifndef COLLIDER_H
#define COLLIDER_H

class Collider : public Component {
protected:
	bool isEnabled;
	float bounds;

public:
	Collider(GameObject* gameObject, Transform* transform);
	~Collider();

	virtual bool Collide() = 0;
};
#endif