#ifndef COLLIDER_H
#define COLLIDER_H

class Collider : public Component {
private:
	Collider *collider;

	static std::vector<Collider*> colliders;

protected:
	bool isEnabled;

protected:
	virtual void start() = 0;
	virtual void update() = 0;

public:
	virtual void Start();
	virtual void Update();

	virtual bool collide(Collider *collider) = 0;

	Collider* Collide();

public:
	Collider(GameObject* gameObject, Transform* transform);
	virtual ~Collider();
};
#endif