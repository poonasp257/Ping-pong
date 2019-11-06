#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Transform;
class Collider;

class Component {
protected:
	virtual void start() = 0;
	virtual void update() = 0;
	
public:
	virtual void Start() { this->start(); }
	virtual void Update() { this->update(); }

	virtual void OnCollisionEnter(Collider *collider) {}
	virtual void OnCollisionStay(Collider *collider) {}
	virtual void OnCollisionExit(Collider *collider) {}

public:
	GameObject* gameObject;
	Transform* transform;

public:
	Component(GameObject* gameObject, Transform* transform);
	virtual ~Component();

	friend class GameObject;
};
#endif