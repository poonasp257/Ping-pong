#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class Component;

class GameObject {
protected:
	std::string name;
	std::vector<Component> components;
	//Model mesh;
	bool isEnable;

public:
	GameObject();
	~GameObject();

	/*void addComponent();
	Component getComponent();

	static GameObject Find(std::string& name);
	static void Destroy(GameObject *gameObject);*/
};
#endif