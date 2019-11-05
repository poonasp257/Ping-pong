#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

class BoxCollider : public Collider {
public:
	BoxCollider(GameObject* gameObject, Transform* transform);
	~BoxCollider();

public:
	bool Collide() { return false; }
};
#endif