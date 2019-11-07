#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

class PlayerController : public Component {
public:
	PlayerController(GameObject *gameObject, Transform *transform);
	virtual ~PlayerController();

public:
	void start();
	void update();
	
	void OnCollisionEnter(Collider *collider) {
		this->gameObject->setActive(false);

	}

	void OnCollisionExit(Collider *collider) {
		this->gameObject->setActive(true);


	}
};
#endif