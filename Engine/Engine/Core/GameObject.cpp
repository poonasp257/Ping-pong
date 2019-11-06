#include "stdafx.h"

std::list<GameObject*> GameObject::gameObjects;

GameObject::GameObject(const std::string& name = "unknown", const std::string& tag = "unknown")
	: isActivated(true), name(name), tag(tag), transform(new Transform(this)), model(new Model) {
	components.push_back(transform);
	gameObjects.push_back(this);
}

GameObject::~GameObject() {
	GameObject::Destoy(this);

	components.clear();
	children.clear();
}

void GameObject::update() {

}

void GameObject::Update() {
	for (auto comp : components) comp->update();
	for (auto child : children) child->update();

	this->update();
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