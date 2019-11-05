#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Transform;

class Component {
protected:
	friend class GameObject;
	GameObject* gameObject;
	Transform* transform;

protected:
	virtual void start() = 0;
	virtual void update() = 0;

public:
	Component(GameObject* gameObject, Transform* transform);
	virtual ~Component();
};
#endif