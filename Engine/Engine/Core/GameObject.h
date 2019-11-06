#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
private:
	std::vector<GameObject*> children;
	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<Transform> transform;

protected:
	bool isActivated;
	std::string name;
	std::string tag;
	std::unique_ptr<Model> model;

protected:
	virtual void update();

public:
	static std::list<GameObject*> gameObjects;

public:
	GameObject(const std::string& name, const std::string& tag);
	~GameObject();

	void Update();

	bool activeSelf() const { return isActivated; }
	void setActive(bool active) { isActivated = active; }

	std::string getName() const { return name; }
	std::string getTag() const { return tag; }
	Transform* getTransform() { return transform.get(); }

	template<typename T>
	T* AddComponent() {
		std::shared_ptr<T> component = std::make_shared<T>(this, this->getTransform());
		if (!dynamic_cast<Component*>(component.get())) {
			component.reset();
			return nullptr;
		}

		component->start();
		components.push_back(component);

		return component.get();
	}

	template<typename T>
	T* GetComponent() {
		for (auto comp : components) {
			auto component = dynamic_cast<T*>(comp.get());
			if (component) return component;
		}

		return nullptr;
	}

public:
	static GameObject* Find(const std::string& name);
	static void Destoy(GameObject *object);
};
#endif