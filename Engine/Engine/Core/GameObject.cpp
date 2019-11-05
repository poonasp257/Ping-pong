#include "stdafx.h"

std::list<GameObject*> GameObject::gameObjects;

GameObject::GameObject(const std::string& name = "unknown", const std::string& tag = "unknown")
	: isActivated(true), name(name), tag(tag), transform(new Transform(this)) {
	components.push_back(transform);
	gameObjects.push_back(this);
}

GameObject::~GameObject() {
	GameObject::Destoy(this);

	components.clear();
	children.clear();
}

void GameObject::update() {
	for (auto comp : components) comp->update();
	for (auto child : children) child->update();
}

template<typename T>
void GameObject::AddComponent() {
	std::shared_ptr<T> component = std::make_shared<T>(this, this->getTransform());
	if (!dynamic_cast<Component*>(component)) {
		component.reset();
		return;
	}

	components.push_back(component);
}

template<typename T>
T* GameObject::GetComponent() {
	for (auto comp : components) {
		auto component = dynamic_cast<T*>(comp.get());
		if (component) return component;
	}

	return nullptr;
}

GameObject* GameObject::Find(const std::string& name) {
	/*char* temp = new char[name.length()];
	strcpy(temp, name.c_str());

	char* tok = strtok(temp, "/");
	for (auto obj : gameObjects) {
		if (obj->name.compare(tok) == 0) return obj;
		else {
			tok = strtok(NULL, "/");
			if (tok == NULL) continue;

			for (auto child : obj->children) return child->Find(tok);
		}
	}*/

	return nullptr;
}

void GameObject::Destoy(GameObject *object) {
	gameObjects.remove(object);
}