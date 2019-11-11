#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

class BoxCollider : public Collider {
private:
	D3DXVECTOR3 oldPosition;
	D3DXVECTOR3 maxVertex;
	D3DXVECTOR3 minVertex;

private:
	void CreateBoundingVolume();
	void AdjustBoxScale();
	void AdjustBoxRotation();

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