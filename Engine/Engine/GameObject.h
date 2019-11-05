#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
private:
	bool isActivated;
	std::string name;
	std::string tag;
	std::vector<GameObject*> children;
	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<Transform> transform;

public:
	static std::list<GameObject*> gameObjects;

public:
	GameObject(const std::string& name, const std::string& tag);
	~GameObject();

	virtual void update();

	bool activeSelf() const { return isActivated; }
	void setActive(bool active) { isActivated = active; }

	std::string getName() const { return name; }
	std::string getTag() const { return tag; }
	Transform* getTransform() { return transform.get(); }

	template<typename T>
	void AddComponent();

	template<typename T>
	T* GetComponent();

public:
	static GameObject* Find(const std::string& name);
	static void Destoy(GameObject *object);
};
#endif