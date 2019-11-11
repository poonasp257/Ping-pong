#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
private:
	std::vector<GameObject*> children;
	std::vector<std::shared_ptr<Component>> components;

	ID3D11Device		*device;
	ID3D11DeviceContext	*deviceContext;

	static std::list<GameObject*> gameObjects;

public:
	bool isActivated;
	std::string name;
	std::string tag;
	std::unique_ptr<Model> model;
	std::shared_ptr<Transform> transform;

public:
	GameObject(ID3D11Device*, ID3D11DeviceContext*, const std::string&, const std::string&);
	~GameObject();

	void update();
	void loadModel(const WCHAR* textureFilename);
	void loadModel(const WCHAR* modelFilename, const WCHAR* textureFilename);

	bool activeSelf() const { return isActivated; }
	void setActive(bool active) { isActivated = active; }

	std::string getName() const { return name; }
	std::string getTag() const { return tag; }
	Transform* getTransform() { return transform.get(); }
	Model* getModel() { return model.get(); }

	template<typename T>
	T* AddComponent() {
		std::shared_ptr<T> component = std::make_shared<T>(this, this->getTransform());
		if (!dynamic_cast<Component*>(component.get())) {
			component.reset();
			return nullptr;
		}
		
		component->Start();
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

	static GameObject* Find(const std::string& name);
	static void Destoy(GameObject *object);

	friend class Collider;
};
#endif