#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

class PlayerController : public Component {
public:
	PlayerController(GameObject *gameObject, Transform *transform);
	virtual ~PlayerController();

public:
	void start();
	void update();
};
#endif