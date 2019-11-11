#include "stdafx.h"

std::list<GameObject*> GameObject::gameObjects;

GameObject::GameObject(ID3D11Device *device, ID3D11DeviceContext *deviceContext,
	const std::string& name = "unknown", const std::string& tag = "unknown")
	: isActivated(true), name(name), tag(tag), 
	transform(new Transform(this)), model(new Model),
	device(device), deviceContext(deviceContext) {
	components.push_back(transform);
	gameObjects.push_back(this);
}

GameObject::~GameObject() {
	GameObject::Destoy(this);

	components.clear();
	children.clear();
}

void GameObject::update() {
	for (auto comp : components) comp->Update();
	for (auto child : children) child->update();

	model->render(deviceContext);
}

void GameObject::loadModel(const WCHAR* textureFilename) {
	model->initialize(device, textureFilename);
}

void GameObject::loadModel(const WCHAR* modelFilename, const WCHAR* textureFilename) {
	model->initialize(device, modelFilename, textureFilename);
}

GameObject* GameObject::Find(const std::string& name) {
	for (auto gameObject : gameObjects) {
		if (gameObject->name == name) {
			return gameObject;
		}
	}

	return nullptr;
}

void GameObject::Destoy(GameObject *object) {
	//gameObjects.remove(object);
	object->setActive(false);
}