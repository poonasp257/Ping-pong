#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

class BoxCollider : public Collider {
private:
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

protected:
	void start();
	void update();

	bool collide(Collider *collider);
	bool collide(BoxCollider *collider);

public:
	BoxCollider(GameObject* gameObject, Transform* transform);
	virtual ~BoxCollider();
};
#endif